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


// TYPE SIZE VERIFICATION 'arena'
SIZECHECKER(arena, STACK_ALIGNMENT);


// GLOBALS
data arena_stack; // of arenas
data arena_index_stack; // of available arena slots

u64   map_count = 0;
u64 unmap_count = 0;


// :::: arena_sanitized_mmap
// :::::::::::::::::: ensures mmap only maps 4096-byte-aligned pages
// :::::::::::::::::: ensures mmap only maps below ARENA_MAX_MEMREQ (4GB - 4KB)

u8* arena_sanitized_mmap(unsigned long* amtptr)
{
    debug_return(!amtptr, "amtptr == nullptr somehow");

    dref(amtptr) = PAGES(AMT2PAGES(dref(amtptr)));

         // NOTE: reusing amtptr
         amtptr = (unsigned long*)(mmap(dref(amtptr)));

    // upon success
    if (u64c(amtptr) != ERROR)
    {
        ++map_count;
        return u8ptr(amtptr);
    }

    // upon fail
    return u8ptr(ARENA_MAP_FAIL);
}


// :::: arena_sanitized_munmap
// :::::::::::::::::: error checks munmap (error format is posix here)
void arena_sanitized_munmap(void* addr, unsigned long length)
{
    debug_warn(addr == nullptr, "address to unmap == nullptr");
    debug_warn(length == 0, "length to unmap == 0");

    if (addr)
    {
        unmap_count += (munmap(addr, length) == 0);
    }
}


// :::: arena_avail_index
// :::::::::::::::::: finds a valid free arena_stack index to map and store arena specs in

u64 arena_avail_index()
{
    static u64 index = 0; // only init. once

           u64 original_index;
        arena* ptr;

    // check cached free indices
    // and return the first one
    if (arena_index_stack.block)
    {
        if (!data_empty(&arena_index_stack))
        {
            return data_popval(&arena_index_stack);
        }
    }

    // scan and return first open slot
    original_index = index;

    do
    {
        // this works
        ptr = data_getval(index, from(arena_stack), arena*);

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
// :::::::::::::::::: an initial mapping initiated to hold all future arenas

u8 arena_grid_start()
{

    u64 workaround = ARENA_GRID_SIZE; // arena_mmap(workaround) .. because macro magic (only this once) .. actually takes &workaround
                                      //   so a variable is declared for the sake of passing in an address, however, the value within
                                      //   will not be increased by mmap because [ ARENA_GRID_SIZE % PAGESIZE == 0 ]

    // create the arena_stack
    arena_stack.block = u64c(arena_mmap(workaround));
    arena_stack.info = data_set_info(0, NOARENA, ARENA_GRID_TYPE);
                                                // ARENA_GRID_TYPE == T16 (sizeof(arena) == 16 bytes)

    workaround = ARENA_GRIDSTACK_SIZE;

    // create the arena_index_stack
    arena_index_stack.block = u64c(arena_mmap(workaround));
    arena_index_stack.info = data_set_info(0, NOARENA, ARENA_GRIDSTACK_TYPE);
                                                    // ARENA_GRIDSTACK_TYPE == T2 if ARENA_GRID_SIZE <  MAX16
                                                    // ARENA_GRIDSTACK_TYPE == T4 if ARENA_GRID_SIZE >= MAX16

    // check the grid
    debug_return(arena_stack.block == 0, "failed to map arena_stack");
    debug_warn(arena_index_stack.block == 0, "warning: no arena arena_index_stack");

    if (!arena_stack.block)
    {
        return EVIL;
    }

    return GOOD;
}



// :::: arena_grid_end
// ::::::::::::::::::::: unmaps all remaining arenas and unmaps the initial mapping of the grid

u8 arena_grid_end()
{
    arena* ptr;
    u64 index;

    // erase all arenas in the grid
    for (index = 0; index < ARENA_MAX_ARENAS; ++index)
    {
        ptr = data_getval(index, from(arena_stack), arena*);
        if (ptr->start)
        {
            arena_munmap(ptr->start, ptr->length);
            ptr->start  = 0;
            ptr->length = 0;
        }
    }

    // erase the arena_stack
    arena_munmap(arena_stack.block, ARENA_GRID_SIZE);
    arena_stack.block = 0;
    arena_stack.info  = 0;

    // erase the arena_index_stack
    if (arena_index_stack.block)
    {
        arena_munmap(arena_index_stack.block, ARENA_GRIDSTACK_SIZE);
        arena_stack.block = 0;
        arena_stack.info  = 0;
    }

    return (unmap_count == map_count);
}


// :::: arena_start
// :::::::::::::::::: gets the starting address of an arena

u8* arena_start(u64 block)
{
        debug_return(block == 0, "block is empty");

    return u8ptr(data_getval(arena_index(block), from(arena_stack), arena*)->start);
}



// :::: arena_map
// :::::::::::::::::: maps an arena and stores its specs into the grid
// :::::::::::::::::: locks memory into ram if specified

u64 arena_mapit(u64 amount, u64 lock_mem_into_ram)
{
        debug_warn(amount > ARENA_MAX_MEMREQ, "amount requested for mapping is greater than ARENA_MAX_MEMREQ");
        debug_warn(amount == 0, "amount to map is zero");

    arena space;
      u64 index;

    if (amount >= ARENA_MAX_MEMREQ)
    {
        amount = (ARENA_MAX_MEMREQ - PAGES(1));
    }
    else if (amount == 0)
    {
        amount = PAGES(1);
    }

    // NOTE: if mmap auto-increases amount passed in
    //       space.length automatically gets updated ... [ internal macro magic only ]
    //                                                   [ not for outer development ]
    space.length = amount;
    space.start = u64c(arena_mmap(space.length));

    if (space.start == ARENA_MAP_FAIL)
    {
        debug_warn(space.start == ARENA_MAP_FAIL, "failed to map arena");
        return ARENA_ZERO_BLOCK;
    }

    // NOTE: signals renegotiation of memory requested
    //       if called from arena_negotiate()
    if (space.start == ARENA_MAP_FAIL)
    {
        return ARENA_ZERO_BLOCK;
    }

    if (lock_mem_into_ram == ARENA_LOCK)
    {
        u64 locked = mlock(u8ptr(space.start), space.length);
        debug_warn(locked == ERROR, "memory was not locked")
    }

    // find available index to store arena specs in
    index = arena_avail_index();

        if (index == ERROR)
        {
            debug_warn(index == ERROR, "out of indices");
            return ARENA_ZERO_BLOCK;
        }

    // store arena specs into the arena_stack index
    data_write(from(space), index, of(arena_stack)); // NOTE: from(arena_stack) == of(arena_stack) == '&arena_stack'
    data_increment(&arena_stack);

    // return an arena-block-descriptor (describes: index-within-global-arena_stack, pages-allocated, bytes-available)
    return ARENA_SET_BLOCK(index, AMT2PAGES(space.length), space.length);
}



// :::: arena_unmap
// :::::::::::::::::: unmaps an arena

void arena_unmapit(u64 block, u64 erase_mem)
{
    arena* ptr;
       u32 status;
       u32 arena_stack_index;

    debug_return(block == 0, "block is empty");
    debug_return(ARENA_PAGES_ALLOCATED(block) == 0, "arena block shows zero pages allocated");

    // extract arena_stackindex from arena block
    arena_stack_index = arena_index(block);

    // get start of arena
    ptr = data_getval(arena_stack_index, from(arena_stack), arena*);

    if (erase_mem == ARENA_ERASE)
    {
         memset(u8ptr(ptr->start), 0xAA, ptr->length);
         memset(u8ptr(ptr->start), 0x55, ptr->length);
        memzero(u8ptr(ptr->start), ptr->length);
    }

    arena_munmap(ptr->start, ptr->length);
                 ptr->start  = 0;
                 ptr->length = 0;

    // declare arena_stack_index available for mapping
    data_pushval(arena_stack_index, to(arena_index_stack));
    data_decrement(&arena_stack);

        // check arena was unmapped
        debug_warn(status != 0, "failed to unmap arena");

    return;
}



// :::: arena_negotiate
// :::::::::::::::::: asks OS for RAM with mmap syscall
// ----------- if memory request through mmap fails
// ----------- calculates request for until more is secured

void arena_negotiate(arena* new, arena* old)
{
    u64 margin;

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
        if (!block)
        {
            debug_warn(block == 0, "block is empty");
            return ARENA_ZERO_BLOCK;
        }

        debug_warn(ARENA_PAGES_ALLOCATED(block) == 0, "arena block shows zero pages allocated");


     arena space;
    arena* aptr;

    aptr = data_getval(arena_index(block), from(arena_stack), arena*);

    // NOTE GROWS by 3 times under 64 KB
    // NOTE GROWS by 2 times between 64 KB and 4 MB
    // NOTE GROWS by 1.5 times when over 4 MB
    arena_negotiate(&space, aptr);

    if (space.start == ERROR)
    {
        debug_warn(TRUE, "failed to negotiate arena growth");
        return ARENA_ZERO_BLOCK;
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
// :::::::::::::::::: returns amount filled

u64 arena_fill(void* ptr, u64 amt, u64* blockptr, u64 growflag)
{
    u64 block;

    debug_return(blockptr == nullptr, "u64* blockptr == nullptr");

    block = dref(blockptr);

    debug_return(block == 0, "block is empty");
    debug_warn(ptr == nullptr, "ptr == nullptr");

    if (ptr && amt)
    {
        if ( (amt > arena_avail(block))  &&  (growflag == ARENA_SHOULD_GROW) )
        {
            do { block = arena_grow(block); } while (amt > arena_avail(block));
        }
        else if (amt > arena_avail(block)) // growflag == ARENA_SHOULD_NOT_GROW
        {
            amt = arena_avail(block);
        }

        memcpy(arena_scribe(block), ptr, amt);

        // NOTE: an increase in arena_inuse(block)
        //          decreases arena_avail(block)
        dref(blockptr) = arena_sub_from_avail(dref(blockptr), amt);
    }

    return amt;
}


// :::: arena_siphon
// :::::::::::::::::: removes some of the raw arena data and returns the updated arena block descriptor
u64 arena_siphon(u64 amt, u64 block)
{
    debug_warn(amt > arena_inuse(block), "amt to siphon is greater than amt in use");

    if (amt > arena_inuse(block))
    {
        amt = arena_inuse(block);
    }

    // NOTE: a decrease in arena_inuse(block)
    //         increases arena_avail(block)
    return (amt) ? arena_add_to_avail(block, amt)
                 : block;
}


// :::: arena_flush
// :::::::::::::::::: returns success or fail

u64 arena_flush(u64 descriptor, u64 block)
{
    u64 tempval = arena_inuse(block);
    if (tempval) // --------->--------->--------->--------->.
    {                                                     /*|*/
        tempval = (write(descriptor, arena_start(block), tempval) == tempval);

        return (tempval == GOOD) ? arena_set_avail(block, arena_width(block))
                                 : block;
    }

    return block;
}
