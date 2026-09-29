#ifndef SYSCALL_MMAP_H
#define SYSCALL_MMAP_H

#include "dep/unite.h"

#if   defined( OS_LINUX )

        #define MAP_PRIVATE 0x02
        #define MAP_ANONYMOUS 0x20
        #define MAP_UNINITIALIZED 0x4000000

#elif defined( OS_GNUHURD )

    #define MAP_PRIVATE 0x02
    #define MAP_ANONYMOUS 0x20
    #define MAP_UNINITIALIZED 0x0

#elif defined( OS_DARWIN )

    #define MAP_PRIVATE 0x02
    #define MAP_ANONYMOUS 0x1000
    #define MAP_UNINITIALIZED 0x0

#elif defined( OS_FREEBSD )

    #define MAP_PRIVATE 0x02
    #define MAP_ANONYMOUS 0x1000
    #define MAP_UNINITIALIZED 0x0

#elif defined( OS_OPENBSD )

    #define MAP_PRIVATE 0x02
    #define MAP_ANONYMOUS 0x1000
    #define MAP_UNINITIALIZED 0x0

#elif defined( OS_NETBSD )

    #define MAP_PRIVATE 0x02
    #define MAP_ANONYMOUS 0x1000
    #define MAP_UNINITIALIZED 0x0

#elif defined( OS_DRAGONFLY )

    #define MAP_PRIVATE 0x02
    #define MAP_ANONYMOUS 0x1000
    #define MAP_UNINITIALIZED 0x0

#elif defined( OS_WINDOWS )

    /* PENDING IMPLEMENTATION */

#elif defined( OS_ANDROID )

    #define MAP_PRIVATE 0x02
    #define MAP_ANONYMOUS 0x20
    #define MAP_UNINITIALIZED 0x0

#elif defined( OS_SOLARIS )

    #define MAP_PRIVATE 0x02
    #define MAP_ANONYMOUS 0x100
    #define MAP_UNINITIALIZED 0x0

#elif defined( OS_AIX )

    #define MAP_PRIVATE 0x02
    #define MAP_ANONYMOUS 0x10
    #define MAP_UNINITIALIZED 0x0

#elif defined( OS_HPUX )

    #define MAP_PRIVATE 0x02
    #define MAP_ANONYMOUS 0x10
    #define MAP_UNINITIALIZED 0x0

#elif defined( OS_MINIX )

    #define MAP_PRIVATE 0x02
    #define MAP_ANONYMOUS 0x1000
    #define MAP_UNINITIALIZED 0x0

#elif defined( OS_QNX )

    #define MAP_PRIVATE 0x02
    #define MAP_PRIVATEANON 0x03
    #define MAP_ANONYMOUS MAP_PRIVATEANON
    #define MAP_NOINIT 0x4000
    #define MAP_UNINITIALIZED MAP_NOINIT

#elif defined( OS_RTEMS )

    #define MAP_PRIVATE 0x02
    #define MAP_ANONYMOUS 0x20
    #define MAP_UNINITIALIZED 0x0

#elif defined( OS_HAIKU )

    #define MAP_PRIVATE 0x02
    #define MAP_ANONYMOUS 0x08
    #define MAP_UNINITIALIZED 0x0

#elif defined( OS_FUCHSIA )

    #define MAP_PRIVATE 0x02
    #define MAP_ANONYMOUS 0x20
    #define MAP_UNINITIALIZED 0x0

#elif defined( OS_EMSCRIPTEN )

    #define MAP_PRIVATE 0x02
    #define MAP_ANONYMOUS 0x20
    #define MAP_UNINITIALIZED 0x0

#else

    #error "MMAP FLAGS: OPERATING SYSTEM NOT FOUND"

#endif



#define PROT_READ  0x1
#define PROT_WRITE 0x2
#define PROT_EXEC  0x4

#define MAP_ANON MAP_ANONYMOUS

#define MMAP_ADDR        (nullptr)
#define MMAP_PROT        (PROT_READ | PROT_WRITE | PROT_EXEC)
#define MMAP_FLAGS       (MAP_PRIVATE | MAP_ANON | MAP_UNINITIALIZED)
#define MMAP_DESCRIPTOR  (-1)
#define MMAP_OFFSET      (0)

#define MMAP2_PAGE_SHIFT_FOR_32BIT_ARCH 12

#endif // SYSCALL_MMAP_H
