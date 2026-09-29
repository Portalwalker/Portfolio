#ifndef OS_H
#define OS_H

// ************************ SOFTWARE OPERATING SYSTEMS ************************ //

/* AIX */
#if defined(_AIX) || defined(__TOS_AIX__)
   #define OS_AIX 1

/* Android (usually implies Linux plus Bionic; compilers define __ANDROID__ ) */
#elif defined(__ANDROID__)
     #define OS_ANDROID 1

/* GNU/Hurd */
#elif defined(__gnu_hurd__) || defined(__GNU__) && defined(__hurd__)
     #define OS_GNUHURD 1

/* Darwin / macOS / iOS (use both __APPLE__ and __MACH__ where commonly defined) */
#elif (defined(__APPLE__) && defined(__MACH__)) || defined(__DARWIN__)
      #define OS_DARWIN 1

/* DragonFly BSD */
#elif defined(__DragonFly__)
     #define OS_DRAGONFLY 1

/* Emscripten (wasm) */
#elif defined(__EMSCRIPTEN__)
     #define OS_EMSCRIPTEN 1

/* FreeBSD */
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
     #define OS_FREEBSD 1

/* Fuchsia */
#elif defined(__Fuchsia__) || defined(__fuchsia__)
     #define OS_FUCHSIA 1

/* HP-UX */
#elif defined(__hpux) || defined(hpux)
     #define OS_HPUX 1

/* Linux */
#elif defined(__linux__) || defined(linux) || defined(__linux)
     #define OS_LINUX 1

/* Minix */
#elif defined(__minix) || defined(__minix__)
     #define OS_MINIX 1

/* NetBSD */
#elif defined(__NetBSD__)
     #define OS_NETBSD 1

/* OpenBSD */
#elif defined(__OpenBSD__)
     #define OS_OPENBSD 1

/* QNX */
#elif defined(__QNX__) || defined(__QNXNTO__)
     #define OS_QNX 1

/* RTEMS */
#elif defined(__rtems) || defined(__rtems__)
     #define OS_RTEMS 1

/* Solaris / SunOS */
#elif defined(__sun) || defined(__sun__) || defined(sun) || defined(__SVR4) || defined(__solaris__)
     #define OS_SOLARIS 1

/* Windows (MSVC and MinGW/Cygwin variants) */
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
     #define OS_WINDOWS 1

/* Unknown OS fallback */
#else
    #define OS_UNKNOWN 1
    #error "OS: UNKNOWN OPERATING SYSTEM"
#endif


#endif // OS_H
