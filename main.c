#include "dep/toHim.h"
#include "dep/arena.h"
#include "dep/trix.h"

void _start()
{
    // start ========================
    u64 status = arena_grid_start();

    // example data
    char* s1 = "Foothold achieved.\n";
    char* s2 = "Secret key captured.\n";
    char* extra = "P@ssw0rd1";
    u64 extralen = memtilnul(extra);

    // output and memory resources
    u64 descriptor = STDOUT;
    u64 a1 = arena_map(PAGES(1));

    // "process" example "data"
    arena_fill(addr(a1), s1, memtilnul(s1), ARENA_CAN_GROW);
    arena_fill(addr(a1), s2, memtilnul(s2), ARENA_CAN_GROW);
    memcpy(arena_scribe(a1), extra, extralen);

    write(descriptor, arena_start(a1), arena_inuse(a1) + extralen);
    write(descriptor, "\n", 1);

    // ======================= finish
    exit(!((status += arena_grid_end()) == 2)); // if 2 successes return 0
                                                // arena_grid_start returns 1 on success
                                                // arena_grid_end returns 1 on success
                                                // weird for now... until
}
