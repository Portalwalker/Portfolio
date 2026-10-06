#include "dep/toHim.h"
#include "dep/arena.h"
#include "dep/trix.h"

void _start()
{
       arena_grid_start();
    /* =================== */


    // example data
    char* s1 = "Foothold achieved.\n";
    char* s2 = "Secret key captured.\n";
    char* extra = "P@ssw0rd1";
    u64 extralen = count_til_nul(extra);

    // output and memory resources
    u64 descriptor = STDOUT;
    u64 a1 = arena_map(PAGES(1));

    // "process" example "data"
    arena_fill(s1, count_til_nul(s1), addr(a1), ARENA_SHOULD_GROW);
    arena_fill(s2, count_til_nul(s2), addr(a1), ARENA_SHOULD_GROW);
    memcpy(arena_scribe(a1), extra, extralen);

    write(descriptor, arena_start(a1), arena_inuse(a1) + extralen);
    write(descriptor, "\n", 1);


    /* ================= */
       arena_grid_end();
                exit(0);
}
