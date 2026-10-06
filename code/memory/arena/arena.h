#ifndef ARENA_H
#define ARENA_H

    #include "dep/unite.h"

    // README => this is the BACKBONE of most of the code base

    // The below macros are oriented towards u64 type utilized as an 'arena descriptor block'
    // e.g. typedef u64 arena_descriptor_block => commonly referred to as 'block'

    // ARENA 'BLOCK' LOGIC [ DO NOT CHANGE WITHOUT UPDATING ARENA_BYTES_AVAILABLE INCR/DECR LOGIC UPDATE ]

    #define ARENA_BITS_ALLOCATED_TO_STORE_INDEX 12 // top      [!] MAX 4096 - 1 ARENAS
    #define ARENA_BITS_ALLOCATED_TO_STORE_PAGES 20 // mid      [!] MAX 1048576 - 1 PAGES (4GB) PER MAPPING
    #define ARENA_BITS_ALLOCATED_TO_STORE_AVAIL 32 // bottom   [!] MAX 4294967296 - 4096 BYTES (4GB) PER ARENA

    #define ARENA_GRID_SLOT_INDEX_MASK u64c(~((BITMASK11 << ARENA_BITS_ALLOCATED_TO_STORE_INDEX) >> ARENA_BITS_ALLOCATED_TO_STORE_INDEX)) // gives top-12-bit-mask
    #define ARENA_BYTES_AVAILABLE_MASK u64c(~((BITMASK11 >> ARENA_BITS_ALLOCATED_TO_STORE_AVAIL) << ARENA_BITS_ALLOCATED_TO_STORE_AVAIL)) // gives bottom-32-bit-mask
    #define ARENA_PAGES_ALLOCATED_MASK u64c(~(ARENA_GRID_SLOT_INDEX_MASK | ARENA_BYTES_AVAILABLE_MASK))                                   // gives mid-20-bit-mask
    #define ARENA_SIMPLE_PAGES_MASK    u64c(ARENA_PAGES_ALLOCATED_MASK >> ARENA_BITS_ALLOCATED_TO_STORE_AVAIL)                            // gives simple-20-bit mask == 0b00000000000011111111111111111111

    #define ARENA_GRID_INDEX(block)      ((u64c(block) & ARENA_GRID_SLOT_INDEX_MASK) >> (ARENA_BITS_ALLOCATED_TO_STORE_PAGES + ARENA_BITS_ALLOCATED_TO_STORE_AVAIL))
    #define ARENA_PAGES_ALLOCATED(block) ((u64c(block) & ARENA_PAGES_ALLOCATED_MASK) >> ARENA_BITS_ALLOCATED_TO_STORE_AVAIL)
    #define ARENA_BYTES_AVAILABLE(block) ( u64c(block) & ARENA_BYTES_AVAILABLE_MASK)

    #define ARENA_SET_GRID_INDEX(index)  u64c(u64c(index) << (ARENA_BITS_ALLOCATED_TO_STORE_PAGES + ARENA_BITS_ALLOCATED_TO_STORE_AVAIL))
    #define ARENA_SET_PAGES_ALLOC(alloc) u64c(u64c(u32c(alloc) & ARENA_SIMPLE_PAGES_MASK) << ARENA_BITS_ALLOCATED_TO_STORE_AVAIL)
    #define ARENA_SET_BYTES_AVAIL(avail) u64c(u32c(avail))

    #define ARENA_SET_BLOCK(index, pages, avail) u64c(ARENA_SET_GRID_INDEX(index) | ARENA_SET_PAGES_ALLOC(pages) | ARENA_SET_BYTES_AVAIL(avail))


    // ARENA GRID LOGIC

    #define ARENA_GRID_SIZE  PAGES(8) // 4096 16-byte blocks
    #define ARENA_GRID_TYPE  T16
    #define ARENA_GRID_TYPE_WIDTH (1 << ARENA_GRID_TYPE)
    #define ARENA_MAX_ARENAS (ARENA_GRID_SIZE / ARENA_GRID_TYPE_WIDTH)

    #if ARENA_MAX_ARENAS < MAX16
        #define ARENA_GRIDSTACK_TYPE T2
    #else
        #define ARENA_GRIDSTACK_TYPE T4
    #endif

    #define ARENA_GRIDSTACK_SIZE (ARENA_MAX_ARENAS << ARENA_GRIDSTACK_TYPE)


    // ARENA GROWTH BEHAVIOR

    #define ARENA_SHOULD_GROW      0x44
    #define ARENA_SHOULD_NOT_GROW  0x00


    // for ease of use
    #define arena_avail(block)    ARENA_BYTES_AVAILABLE(block)
    #define arena_full(block)     (!arena_avail(block))
    #define arena_empty(block)    ( arena_avail(block) == arena_width(block))
    #define arena_index(block)    ARENA_GRID_INDEX(block)
    #define arena_pages(block)    ARENA_PAGES_ALLOCATED(block)
    #define arena_width(block)    PAGES(ARENA_PAGES_ALLOCATED(block))
    #define arena_inuse(block)    (arena_width(block) - arena_avail(block))
    #define arena_scribe(block)   (arena_start(block) + arena_inuse(block))

    // setters
    #define arena_set_index(block, index) (((~(ARENA_GRID_SLOT_INDEX_MASK)) & (block)) | ARENA_SET_GRID_INDEX(index))  // (slower) ARENA_SET_BLOCK((index), arena_pages(block), arena_avail(block))
    #define arena_set_pages(block, pages) (((~(ARENA_PAGES_ALLOCATED_MASK)) & (block)) | ARENA_SET_PAGES_ALLOC(pages)) // (slower) ARENA_SET_BLOCK(arena_index(block), (pages), arena_avail(block))
    #define arena_set_avail(block, avail) (((~(ARENA_BYTES_AVAILABLE_MASK)) & (block)) | ARENA_SET_BYTES_AVAIL(avail)) // (slower) ARENA_SET_BLOCK(arena_index(index), arena_pages(block), (avail))

    // modifiers - NOTE: the lower 32 bits of member 'block' hold available bytes
    #define arena_add_to_avail(block, amt)   ((block) + (amt))
    #define arena_sub_from_avail(block, amt) ((block) - (amt))

    // more complex
    #define arena_amt_avail(amt, block)  ((amt) <= arena_avail(block))

    // wrappers
    #define arena_mmap(amt)         arena_sanitized_mmap(addr(amt)) // [!] invisible change (will actually update amount)
    #define arena_munmap(addr, len) arena_sanitized_munmap(voidptr(addr), (len))

    #define ARENA_LOCK   0xACEACEACEACEACE7
    #define ARENA_NOLOCK 0xE720E720E720E720

    #define ARENA_ERASE   0xAAAAAAAA55555555
    #define ARENA_NOERASE 0xFFFFFFFF11111111

    #define arena_special_map(amt)   arena_mapit(u64c(amt), ARENA_LOCK)
    #define arena_map(amt)           arena_mapit(u64c(amt), ARENA_NOLOCK)

    #define arena_special_unmap(block) arena_unmapit((block), ARENA_ERASE)
    #define arena_unmap(block)         arena_unmapit((block), ARENA_NOERASE)

    #define ARENA_MAP_FAIL   0
    #define ARENA_ZERO_BLOCK 0

    // arena
    //       start  => pointer to mapped memory returned from mmap syscall
    //       length => length (in bytes) of arena
    typedef struct
    {
        u64 start;
        u64 length;
    } arena;

    extern u64   map_count;
    extern u64 unmap_count;

     // initialization
     u64 arena_avail_index();
      u8 arena_grid_start();
      u8 arena_grid_end();

     // external use
     u64 arena_mapit(u64 amount, u64 lock_mem_into_ram);
    void arena_unmapit(u64 block, u64 erase_mem);
     u64 arena_grow(u64 block);
     u64 arena_fill(void* ptr, u64 amt, u64* blockptr, u64 growflag);
     u64 arena_siphon(u64 block, u64 amt);
     u64 arena_flush(u64 descriptor, u64 block);
     u8* arena_start(u64 block);

    // internal use
    void arena_negotiate(arena* new, arena* old);


#endif // ARENA_H
