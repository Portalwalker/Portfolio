#include "dep/arena.h"
#include "dep/data.h"
#include "dep/trix.h"
#include "dep/math.h"



// ARENA CONSTRAINTS & HELPER MACROS

#define ABOVE_64KB(amt)  (((amt) / (64 * KILOBYTE)) != 0)
#define ABOVE_4MB(amt)   (((amt) / ( 4 * MEGABYTE)) != 0)

#define AMT_IFTRUE(amt, condition) ((amt) * ((condition) == TRUE))

#define ARENA_MAX_MEMREQ    (u64c(GIGABYTE) * u64c(4))
#define ARENA_GROW(amt)     (AMT_IFTRUE((amt << 1) + amt, !ABOVE_64KB(amt)) + AMT_IFTRUE(amt, ABOVE_64KB(amt)) + AMT_IFTRUE(amt >> 1, ABOVE_4MB(amt)))


// compiler trick to force 'arena' to be 16 bytes
SIZECHECKER(arena, STACK_ALIGNMENT);


// GLOBALS
data gridstack; // of arenas
data gridindexer; // available arena slots


// :::: arena_sanitized_mmap
// ::::::::::::::::::::::::::: ensures mmap only maps 4096-byte-aligned pages
// ::::::::::::::::::::::::::: ensures mmap only maps below ARENA_MAX_MEMREQ (4GB - 4KB)

u8* arena_sanitized_mmap(unsigned long* amtptr)
{
    debug_return(!amtptr, "amtptr == nullptr somehow");

    dref(amtptr) = PAGES(AMT2PAGES(dref(amtptr)));

    debug_return(dref(amtptr) > ARENA_MAX_MEMREQ, "amount requested surpasses ARENA_MAX_MEMREQ");
    debug_return(dref(amtptr) == 0, "amount requested is zero");

    return u8ptr(mmap(dref(amtptr)));
}


// :::: arena_sanitized_munmap
// ::::::::::::::::::::::::::::: error checks munmap (error format is posix here)

unsigned long arena_sanitized_munmap(void* addr, unsigned long length)
{
    debug_warn(addr == nullptr, "address to unmap == nullptr");
    debug_warn(length == 0, "length to unmap == 0");

    return (addr) ? munmap(addr, length)
                  : 0;
}


// :::: arena_avail_gridstack_index
// :::::::::::::::::::::::::::::::::: finds a valid free gridstack index to map and store arena specs in

u64 arena_avail_gridstack_index()
{
    static u64 original_index;
    static u64 index = 0; // only init. once
    static arena* ptr;

    if (!data_empty(&gridindexer))
    {
        return data_popval(&gridindexer);
    }

    // scan and return first open slot
    original_index = index;

    do
    {
        // this works
        ptr = cast(data_getval(&gridstack, index), arena*);

        // if empty/available
        if (!ptr->start)
        {
            return index;
        }

        // continue scanning
        index = (index + 1) % ARENA_MAX_ARENAS;

      // the circular-modulo increment optimizes the scan for available indices
    } while (index != original_index);

    return ERROR;
}



// :::: arena_grid_start
// ::::::::::::::::::::::: an initial mapping initiated to hold all future arenas

u8 arena_grid_start()
{

    u64 workaround = ARENA_GRID_SIZE; // arena_mmap(workaround) .. because macro magic (only this once) .. actually takes &workaround
                                      //   so a variable is declared for the sake of passing in an address, however, the value within
                                      //   will not be increased by mmap because [ ARENA_GRID_SIZE % PAGESIZE == 0 ]

    // create the gridstack
    gridstack.block = u64c(arena_mmap(workaround));
    gridstack.info = data_set_info(0, NOARENA, ARENA_GRID_TYPE);
                                                // ARENA_GRID_TYPE == T16 (sizeof(arena) == 16 bytes)

    workaround = ARENA_GRIDSTACK_SIZE;

    // create the gridindexer
    gridindexer.block = u64c(arena_mmap(workaround));
    gridindexer.info = data_set_info(0, NOARENA, ARENA_GRIDSTACK_TYPE);
                                                    // ARENA_GRIDSTACK_TYPE == T2 if ARENA_GRID_SIZE <  MAX16
                                                    // ARENA_GRIDSTACK_TYPE == T4 if ARENA_GRID_SIZE >= MAX16

    // check the grid
    debug_return(gridstack.block == 0, "failed to map gridstack");
    debug_return(gridindexer.block == 0, "failed to map gridindexer");

    // returns EVIL == 0 if mmap fails
    return GOOD;
}



// :::: arena_grid_end
// ::::::::::::::::::::: unmaps all remaining arenas and unmaps the initial mapping of the grid

u8 arena_grid_end()
{
    arena* ptr;
    u32 status = 0;
    u32 i;

    // erase all arenas in the grid
    for (i = 0; i < ARENA_MAX_ARENAS; ++i)
    {
        ptr = cast(data_getval(&gridstack, i), arena*);
        if (ptr->start)
        {
            status += arena_munmap(ptr->start, ptr->length);
            ptr->start  = 0;
            ptr->length = 0;
        }
    }

    // erase the gridstack
    status += arena_munmap(gridstack.block, ARENA_GRID_SIZE);
    gridstack.block = 0;
    gridstack.info  = 0;

    // erase the gridindexer
    status += arena_munmap(gridindexer.block, ARENA_GRIDSTACK_SIZE);
    gridstack.block = 0;
    gridstack.info  = 0;

    return (status == 0) ? GOOD : EVIL;
}


// :::: arena_start
// :::::::::::::::::: gets the starting address of an arena

u8* arena_start(u64 block)
{
        debug_return(block == 0, "block is empty");

    return u8ptr(cast(data_getval(&gridstack, arena_index(block)), arena*)->start);
}



// :::: arena_map
// :::::::::::::::: maps an arena and stores its specs into the grid
// :::::::::::::::: locks memory into ram if specified

u64 arena_mapit(u64 amount, u64 lock_mem_into_ram)
{
    static arena space;
    static u64 index;

    debug_return(amount > ARENA_MAX_MEMREQ, "amount requested for mapping is greater than ARENA_MAX_MEMREQ");
    debug_return(amount == 0, "amount to map is zero");

    if (amount == ARENA_MAX_MEMREQ)
    {
        amount -= PAGESIZE;
    }

    // NOTE: if mmap auto-increases amount passed in
    //       space.length automatically gets updated ... macro magic
    space.length = amount;
    space.start = u64c(arena_mmap(space.length));

    // check mapping (returning completely zero arena-descriptor will indicate failure)
    debug_return(!space.start, "failed to map arena");

    // signals renegotiation of memory requested
    if (!space.start)
    {
        return 0;
    }

    if (lock_mem_into_ram == ARENA_LOCK)
    {
        u64 locked = mlock(u8ptr(space.start), space.length);
        debug_warn(locked == ERROR, "memory was not locked")
    }

    // find available index to store arena specs in
    index = arena_avail_gridstack_index();

        debug_return(index == ERROR, "out of indices");

    // store arena specs into the gridstack index
    data_write(&space, &gridstack, index);
    data_increment(&gridstack);

    // return an arena-descriptor (describes: index-within-global-gridstack, pages-allocated, bytes-available)
    return ARENA_SET_BLOCK(index, AMT2PAGES(space.length), space.length);
}



// :::: arena_unmap
// :::::::::::::::::: unmaps an arena

u64 arena_unmapit(u64 block, u64 erase_mem)
{
    static arena* ptr;
    static u32 status;
    static u32 gridstack_index;

    debug_return(block == 0, "block is empty");
    debug_return(ARENA_PAGES_ALLOCATED(block) == 0, "arena block shows zero pages allocated");

    // extract gridstackindex from arena block
    gridstack_index = arena_index(block);

    // get start of arena
    ptr = cast(data_getval(&gridstack, gridstack_index), arena*);

    if (erase_mem == ARENA_ERASE)
    {
         memset(u8ptr(ptr->start), 0xAA, ptr->length);
         memset(u8ptr(ptr->start), 0x55, ptr->length);
        memzero(u8ptr(ptr->start), ptr->length);
    }

    status = arena_munmap(ptr->start, ptr->length);
    ptr->start = 0;
    ptr->length = 0;

        // check arena was unmapped
        debug_warn(status != 0, "failed to unmap arena");

    // declare gridstack_index available for mapping
    data_pushval(gridstack_index, &gridindexer);
    data_decrement(&gridstack);

    return status;
}



// :::: arena_negotiate
// :::::::::::::::::::::: asks OS for RAM with mmap syscall
// :::::::::::::::::::::: if memory request through mmap fails
// :::::::::::::::::::::: calculates request for until more is secured

void arena_negotiate(arena* new, arena* old)
{
    static u64 margin;

    debug_return(new == nullptr, "arena* new == nullptr");
    debug_return(old == nullptr, "arena* old == nullptr");

    new->length = ARENA_GROW(old->length);

    debug_warn(new->length <= old->length, "new length is less than or equal to old length");
    debug_warn((new->length % PAGESIZE) != 0, "new length is not a multiple of PAGESIZE [ 4 KB ]");

    margin = absval(new->length - old->length);
    new->start = ERROR;

    while (new->start == ERROR && margin >= PAGESIZE)
    {

        new->length = old->length + margin;      // request memory
        new->start = u64c(arena_mmap(new->length));    // divide the request by 2
                                                 // for each mapping failure until
        margin >>= 1; // div by 2                // a one-page memory request fails

    }
}



// :::: arena_grow
// ::::::::::::::::: grows an arena and returns a new arena block

u64 arena_grow(u64 block)
{
    static arena space;
           arena* aptr;

    debug_return(block == 0, "block is empty");
    debug_warn(ARENA_PAGES_ALLOCATED(block) == 0, "arena block shows zero pages allocated");

    aptr = cast(data_getval(&gridstack, arena_index(block)), arena*);

    // NOTE GROWS by 3 times under 64 KB
    // NOTE GROWS by 2 times between 64 KB and 4 MB
    // NOTE GROWS by 1.5 times when over 4 MB
    arena_negotiate(&space, aptr);

    if (space.start == ERROR)
    {
        debug_warn(TRUE, "failed to negotiate arena growth");
        return ARENA_MAP_FAIL;
    }
                                                                      // what was available before + new memory increase
    block = ARENA_SET_BLOCK(arena_index(block), AMT2PAGES(space.length), arena_avail(block) + (space.length - aptr->length));

    memcpy(voidptr(space.start), voidptr(aptr->start), aptr->length);

    arena_munmap(aptr->start, aptr->length);

    aptr->start = space.start;
    aptr->length = space.length;

    return block;
}



// :::: arena_fill
// ::::::::::::::::: fills an arena with

u64 arena_fill(u64* blockptr, void* ptr, u64 amt, u64 growflag)
{
    static u64 block;

    debug_return(blockptr == nullptr, "u64* blockptr == nullptr");

    block = dref(blockptr);

    debug_return(block == 0, "block is empty");
    debug_warn(ptr == nullptr, "ptr == nullptr");

    if (ptr && amt)
    {
        if ( (amt > arena_avail(block))  &&  (growflag == ARENA_CAN_GROW) )
        {
            do
            {

                block = arena_grow(block);

            } while (amt > arena_avail(block));
        }
        else if (amt > arena_avail(block)) // && growflag == ARENA_CANNOT_GROW
        {
            amt = arena_avail(block);
        }

        memcpy(arena_scribe(block), ptr, amt);

        // NOTE: bit layout defined in arena.c supports such arithmetic
        //       length increase => arena availability decrease
        dref(blockptr) = block - amt;
    }
    else
    {
        amt = 0;
    }

    // return how much was filled
    return amt;
}
