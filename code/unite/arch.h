#ifndef ARCH_H
#define ARCH_H

// ************************ HARDWARE ARCHITECTURE ************************ //

/* 64-bit x86 (AMD64 / x86-64) */
#if defined(x86_64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(__amd64__) || defined(__amd64)

   #define ARCH_X8664 1
   #define ARCH_BITS 64

/* 32-bit x86 (i386 / i486 / i586 / i686) */
#elif defined(__i386__) || defined(__i386) || defined(_M_IX86) || defined(__ia32__) || defined(__INTEL__)

     #define ARCH_I386 1
     #define ARCH_BITS 32

/* ARM 64-bit (AArch64) */
#elif defined(__aarch64__) || defined(__aarch64) || defined(_M_ARM64) || defined(__arm64__)

     #define ARCH_ARM64 1
     #define ARCH_BITS 64

/* ARM 32-bit (ARM/ARMv7/ARMv6 etc.) */
#elif defined(__arm__) || defined(__arm) || defined(_M_ARM) || defined(__ARMEL__) || defined(__ARMEB__)

     #define ARCH_ARM32 1
     #define ARCH_BITS 32

/* MIPS N64 64-bit NOTE: O64 IS NOT NORMALLY SUPPORTED */
#elif defined(_ABIN64) || defined(__mips64) || defined(__mips64_)

     #define ARCH_MIPS_N64 1
     #define ARCH_BITS 64

/* MIPS N32|O32 32-bit */
#elif defined(__mips__) || defined(__mips) || defined(__MIPS__)

    /* MIPS N32 */
    #if defined(_ABIN32)

       #define ARCH_MIPS_N32 1

    /* MIPS O32 */
    #elif defined(_ABIO32)

         #define ARCH_MIPS_O32 1

    #endif

#define ARCH_BITS 32

/* PowerPC 64-bit */
#elif defined(__powerpc64__) || defined(__ppc64__)

     #define ARCH_PPC64 1
     #define ARCH_BITS 64

/* PowerPC 32-bit */
#elif defined(__powerpc__) || defined(__powerpc) || defined(__ppc__) || defined(__PPC__) || defined(_M_PPC)

     #define ARCH_PPC32 1
     #define ARCH_BITS 32

/* SPARC 64-bit */
#elif defined(__sparc_v9__)

     #define ARCH_SPARC64 1
     #define ARCH_BITS 64

/* SPARC 32-bit */
#elif defined(__sparc__) || defined(__sparc)

     #define ARCH_SPARC32 1
     #define ARCH_BITS 32

/* s390x (IBM System z 64-bit) */
#elif defined(__s390x__)

     #define ARCH_S390x 1
     #define ARCH_BITS 64

/* s390 (IBM System z 31-bit => uses 32bit registers) */
#elif defined(__s390__)

     #define ARCH_S390 1
     #define ARCH_BITS 32

/* RISC-V */
#elif defined(__riscv) || defined(__riscv__) || defined(__riscv_xlen)

     #define ARCH_RISCV 1
     #define ARCH_BITS __riscv_xlen

/* HPPA (PA-RISC) */
#elif defined(__hppa) || defined(__hppa__)

     #define ARCH_HPPA 1
     #define ARCH_BITS 32

/* Alpha */
#elif defined(__alpha__) || defined(__alpha)

     #define ARCH_ALPHA 1
     #define ARCH_BITS 32

/* Itanium / IA-64 */
#elif defined(__ia64__) || defined(__ia64) || defined(_M_IA64)

     #define ARCH_IA64 1
     #define ARCH_BITS 64

/* AVR32 */
#elif defined(__avr32__)

     #define ARCH_AVR32 1
     #define ARCH_BITS 32

/* AVR (8-bit microcontroller) */
#elif defined(__AVR__)

     #define ARCH_AVR8 1
     #define ARCH_BITS 8

/* MicroBlaze */
#elif defined(__microblaze__) || defined(__MICROBLAZE__)

     #define ARCH_MICROBLAZE 1
     #define ARCH_BITS 32

/* Tile (Tilera) */
#elif defined(__tile__) || defined(__tile)

     #define ARCH_TILE 1
     #define ARCH_BITS 32

/* SH (SuperH) */
#elif defined(__sh__) || defined(__SH__)

     #define ARCH_SH 1
     #define ARCH_BITS 32

/* Generic fallback: unknown architecture */
#else
     #error "ARCHITECTURE: UNKNOWN ARCHITECTURE"
     #define ARCH_UNKNOWN 1
     #define ARCH_BITS 0
#endif

/* LISTS

#if   defined( ARCH_X8664 )
#elif defined( ARCH_I386 )
#elif defined( ARCH_ARM64 )
#elif defined( ARCH_ARMEABI )
#elif defined( ARCH_ARMOABI )
#elif defined( ARCH_MIPSN64 )
#elif defined( ARCH_MIPSN32 )
#elif defined( ARCH_MIPSO32 )
#elif defined( ARCH_PPC64 )
#elif defined( ARCH_PPC32 )
#elif defined( ARCH_SPARC64 )
#elif defined( ARCH_SPARC32 )
#elif defined( ARCH_S390x )
#elif defined( ARCH_S390 )
#elif defined( ARCH_RISCV )
#elif defined( ARCH_AVR32 )
#elif defined( ARCH_AVR8 )
#else
#endif

*/


#endif // ARCH_H
