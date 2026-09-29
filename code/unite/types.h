#ifndef TYPES_H
#define TYPES_H

    /* Exact-width integer types
     * Portable Snippets - https://github.com/nemequ/portable-snippets
     * Created by Evan Nemerson <evan@nemerson.com>
     *
     *   To the extent possible under law, the authors have waived all
     *   copyright and related or neighboring rights to this code.  For
     *   details, see the Creative Commons Zero 1.0 Universal license at
     *   https://creativecommons.org/publicdomain/zero/1.0/
     *
     * This header tries to define psnip_(u)int(8|16|32|64)_t to
     * appropriate types given your system.  For most systems this means
     * including <stdint.h> and adding a few preprocessor definitions, but
     * when that isn't an option things get a bit more complicated.
     */

    #if !defined(PSNIP_EXACT_INT_H)
    #  define PSNIP_EXACT_INT_H

    #  if defined(psnip_int8_t)
    #    undef psnip_int8_t
    #  endif
    #  if defined(psnip_uint8_t)
    #    undef psnip_uint8_t
    #  endif
    #  if defined(psnip_int16_t)
    #    undef psnip_int16_t
    #  endif
    #  if defined(psnip_uint16_t)
    #    undef psnip_uint16_t
    #  endif
    #  if defined(psnip_int32_t)
    #    undef psnip_int32_t
    #  endif
    #  if defined(psnip_uint32_t)
    #    undef psnip_uint32_t
    #  endif
    #  if defined(psnip_int64_t)
    #    undef psnip_int64_t
    #  endif
    #  if defined(psnip_uint64_t)
    #    undef psnip_uint64_t
    #  endif

    #  if !defined(PSNIP_EXACT_INT_HAVE_STDINT)
    #    if defined(_STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
    #      define PSNIP_EXACT_INT_HAVE_STDINT
    #    elif defined(__has_include)
    #      if __has_include(<stdint.h>)
    #        define PSNIP_EXACT_INT_HAVE_STDINT
    #      endif
    #    elif \
          defined(HAVE_STDINT_H) || \
          defined(_STDINT_H_INCLUDED) || \
          defined(_STDINT_H) || \
          defined(_STDINT_H_)
    #      define PSNIP_EXACT_INT_HAVE_STDINT
    #    elif \
          (defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5))) || \
          (defined(_MSC_VER) && (_MSC_VER >= 1600)) || \
          (defined(__SUNPRO_C) && (__SUNPRO_C >= 0x570)) || \
          (defined(__WATCOMC__) && (__WATCOMC__ >= 1250))
    #      define PSNIP_EXACT_INT_HAVE_STDINT
    #    endif
    #  endif

    #  if \
      defined(__INT8_TYPE__) && defined(__INT16_TYPE__) && defined(__INT32_TYPE__) && defined(__INT64_TYPE__) && \
      defined(__UINT8_TYPE__) && defined(__UINT16_TYPE__) && defined(__UINT32_TYPE__) && defined(__UINT64_TYPE__)
    #    define psnip_int8_t   __INT8_TYPE__
    #    define psnip_int16_t  __INT16_TYPE__
    #    define psnip_int32_t  __INT32_TYPE__
    #    define psnip_int64_t  __INT64_TYPE__
    #    define psnip_uint8_t  __UINT8_TYPE__
    #    define psnip_uint16_t __UINT16_TYPE__
    #    define psnip_uint32_t __UINT32_TYPE__
    #    define psnip_uint64_t __UINT64_TYPE__
    #  elif defined(PSNIP_EXACT_INT_HAVE_STDINT)
    #    include <stdint.h>
    #    if !defined(psnip_int8_t)
    #      define psnip_int8_t int8_t
    #    endif
    #    if !defined(psnip_uint8_t)
    #      define psnip_uint8_t uint8_t
    #    endif
    #    if !defined(psnip_int16_t)
    #      define psnip_int16_t int16_t
    #    endif
    #    if !defined(psnip_uint16_t)
    #      define psnip_uint16_t uint16_t
    #    endif
    #    if !defined(psnip_int32_t)
    #      define psnip_int32_t int32_t
    #    endif
    #    if !defined(psnip_uint32_t)
    #      define psnip_uint32_t uint32_t
    #    endif
    #    if !defined(psnip_int64_t)
    #      define psnip_int64_t int64_t
    #    endif
    #    if !defined(psnip_uint64_t)
    #      define psnip_uint64_t uint64_t
    #    endif
    #  elif defined(_MSC_VER)
    #    if !defined(psnip_int8_t)
    #      define psnip_int8_t __int8
    #    endif
    #    if !defined(psnip_uint8_t)
    #      define psnip_uint8_t unsigned __int8
    #    endif
    #    if !defined(psnip_int16_t)
    #      define psnip_int16_t __int16
    #    endif
    #    if !defined(psnip_uint16_t)
    #      define psnip_uint16_t unsigned __int16
    #    endif
    #    if !defined(psnip_int32_t)
    #      define psnip_int32_t __int32
    #    endif
    #    if !defined(psnip_uint32_t)
    #      define psnip_uint32_t unsigned __int32
    #    endif
    #    if !defined(psnip_int64_t)
    #      define psnip_int64_t __int64
    #    endif
    #    if !defined(psnip_uint64_t)
    #      define psnip_uint64_t unsigned __int64
    #    endif
    #  else
    #    include <limits.h>
    #    if !defined(psnip_int8_t)
    #      if defined(CHAR_MIN) && defined(CHAR_MAX) && (CHAR_MIN == (-127-1)) && (CHAR_MAX == 127)
    #        define psnip_int8_t char
    #      elif defined(SHRT_MIN) && defined(SHRT_MAX) && (SHRT_MIN == (-127-1)) && (SHRT_MAX == 127)
    #        define psnip_int8_t short
    #      elif defined(INT_MIN) && defined(INT_MAX) && (INT_MIN == (-127-1)) && (INT_MAX == 127)
    #        define psnip_int8_t int
    #      elif defined(LONG_MIN) && defined(LONG_MAX) && (LONG_MIN == (-127-1)) && (LONG_MAX == 127)
    #        define psnip_int8_t long
    #      elif defined(LLONG_MIN) && defined(LLONG_MAX) && (LLONG_MIN == (-127-1)) && (LLONG_MAX == 127)
    #        define psnip_int8_t long long
    #      else
    #        error Unable to locate 8-bit signed integer type.
    #      endif
    #    endif
    #    if !defined(psnip_uint8_t)
    #      if defined(UCHAR_MAX) && (UCHAR_MAX == 255)
    #        define psnip_uint8_t unsigned char
    #      elif defined(USHRT_MAX) && (USHRT_MAX == 255)
    #        define psnip_uint8_t unsigned short
    #      elif defined(UINT_MAX) && (UINT_MAX == 255)
    #        define psnip_uint8_t unsigned int
    #      elif defined(ULONG_MAX) && (ULONG_MAX == 255)
    #        define psnip_uint8_t unsigned long
    #      elif defined(ULLONG_MAX) && (ULLONG_MAX == 255)
    #        define psnip_uint8_t unsigned long long
    #      else
    #        error Unable to locate 8-bit unsigned integer type.
    #      endif
    #    endif
    #    if !defined(psnip_int16_t)
    #      if defined(CHAR_MIN) && defined(CHAR_MAX) && (CHAR_MIN == (-32767-1)) && (CHAR_MAX == 32767)
    #        define psnip_int16_t char
    #      elif defined(SHRT_MIN) && defined(SHRT_MAX) && (SHRT_MIN == (-32767-1)) && (SHRT_MAX == 32767)
    #        define psnip_int16_t short
    #      elif defined(INT_MIN) && defined(INT_MAX) && (INT_MIN == (-32767-1)) && (INT_MAX == 32767)
    #        define psnip_int16_t int
    #      elif defined(LONG_MIN) && defined(LONG_MAX) && (LONG_MIN == (-32767-1)) && (LONG_MAX == 32767)
    #        define psnip_int16_t long
    #      elif defined(LLONG_MIN) && defined(LLONG_MAX) && (LLONG_MIN == (-32767-1)) && (LLONG_MAX == 32767)
    #        define psnip_int16_t long long
    #      else
    #        error Unable to locate 16-bit signed integer type.
    #      endif
    #    endif
    #    if !defined(psnip_uint16_t)
    #      if defined(UCHAR_MAX) && (UCHAR_MAX == 65535)
    #        define psnip_uint16_t unsigned char
    #      elif defined(USHRT_MAX) && (USHRT_MAX == 65535)
    #        define psnip_uint16_t unsigned short
    #      elif defined(UINT_MAX) && (UINT_MAX == 65535)
    #        define psnip_uint16_t unsigned int
    #      elif defined(ULONG_MAX) && (ULONG_MAX == 65535)
    #        define psnip_uint16_t unsigned long
    #      elif defined(ULLONG_MAX) && (ULLONG_MAX == 65535)
    #        define psnip_uint16_t unsigned long long
    #      else
    #        error Unable to locate 16-bit unsigned integer type.
    #      endif
    #    endif
    #    if !defined(psnip_int32_t)
    #      if defined(CHAR_MIN) && defined(CHAR_MAX) && (CHAR_MIN == (-2147483647-1)) && (CHAR_MAX == 2147483647)
    #        define psnip_int32_t char
    #      elif defined(SHRT_MIN) && defined(SHRT_MAX) && (SHRT_MIN == (-2147483647-1)) && (SHRT_MAX == 2147483647)
    #        define psnip_int32_t short
    #      elif defined(INT_MIN) && defined(INT_MAX) && (INT_MIN == (-2147483647-1)) && (INT_MAX == 2147483647)
    #        define psnip_int32_t int
    #      elif defined(LONG_MIN) && defined(LONG_MAX) && (LONG_MIN == (-2147483647-1)) && (LONG_MAX == 2147483647)
    #        define psnip_int32_t long
    #      elif defined(LLONG_MIN) && defined(LLONG_MAX) && (LLONG_MIN == (-2147483647-1)) && (LLONG_MAX == 2147483647)
    #        define psnip_int32_t long long
    #      else
    #        error Unable to locate 32-bit signed integer type.
    #      endif
    #    endif
    #    if !defined(psnip_uint32_t)
    #      if defined(UCHAR_MAX) && (UCHAR_MAX == 4294967295)
    #        define psnip_uint32_t unsigned char
    #      elif defined(USHRT_MAX) && (USHRT_MAX == 4294967295)
    #        define psnip_uint32_t unsigned short
    #      elif defined(UINT_MAX) && (UINT_MAX == 4294967295)
    #        define psnip_uint32_t unsigned int
    #      elif defined(ULONG_MAX) && (ULONG_MAX == 4294967295)
    #        define psnip_uint32_t unsigned long
    #      elif defined(ULLONG_MAX) && (ULLONG_MAX == 4294967295)
    #        define psnip_uint32_t unsigned long long
    #      else
    #        error Unable to locate 32-bit unsigned integer type.
    #      endif
    #    endif
    #    if !defined(psnip_int64_t)
    #      if defined(CHAR_MIN) && defined(CHAR_MAX) && (CHAR_MIN == (-9223372036854775807LL-1)) && (CHAR_MAX == 9223372036854775807LL)
    #        define psnip_int64_t char
    #      elif defined(SHRT_MIN) && defined(SHRT_MAX) && (SHRT_MIN == (-9223372036854775807LL-1)) && (SHRT_MAX == 9223372036854775807LL)
    #        define psnip_int64_t short
    #      elif defined(INT_MIN) && defined(INT_MAX) && (INT_MIN == (-9223372036854775807LL-1)) && (INT_MAX == 9223372036854775807LL)
    #        define psnip_int64_t int
    #      elif defined(LONG_MIN) && defined(LONG_MAX) && (LONG_MIN == (-9223372036854775807LL-1)) && (LONG_MAX == 9223372036854775807LL)
    #        define psnip_int64_t long
    #      elif defined(LLONG_MIN) && defined(LLONG_MAX) && (LLONG_MIN == (-9223372036854775807LL-1)) && (LLONG_MAX == 9223372036854775807LL)
    #        define psnip_int64_t long long
    #      else
    #        error Unable to locate 64-bit signed integer type.
    #      endif
    #    endif
    #    if !defined(psnip_uint64_t)
    #      if defined(UCHAR_MAX) && (UCHAR_MAX == 18446744073709551615ULL)
    #        define psnip_uint64_t unsigned char
    #      elif defined(USHRT_MAX) && (USHRT_MAX == 18446744073709551615ULL)
    #        define psnip_uint64_t unsigned short
    #      elif defined(UINT_MAX) && (UINT_MAX == 18446744073709551615ULL)
    #        define psnip_uint64_t unsigned int
    #      elif defined(ULONG_MAX) && (ULONG_MAX == 18446744073709551615ULL)
    #        define psnip_uint64_t unsigned long
    #      elif defined(ULLONG_MAX) && (ULLONG_MAX == 18446744073709551615ULL)
    #        define psnip_uint64_t unsigned long long
    #      else
    #        error Unable to locate 64-bit unsigned integer type.
    #      endif
    #    endif
    #  endif
    #endif

    typedef  psnip_uint8_t  u8;
    typedef psnip_uint16_t u16;
    typedef psnip_uint32_t u32;
    typedef psnip_uint64_t u64;
    typedef   psnip_int8_t  i8;
    typedef  psnip_int16_t i16;
    typedef  psnip_int32_t i32;
    typedef  psnip_int64_t i64;

    #ifndef uint8_t
        typedef  psnip_uint8_t  uint8_t;
        typedef psnip_uint16_t uint16_t;
        typedef psnip_uint32_t uint32_t;
        typedef psnip_uint64_t uint64_t;
        typedef   psnip_int8_t   int8_t;
        typedef  psnip_int16_t  int16_t;
        typedef  psnip_int32_t  int32_t;
        typedef  psnip_int64_t  int64_t;
    #endif

    #ifndef size_t
        #  if __SIZEOF_POINTER__ == 4
              typedef  u32   size_t;
              typedef  i32  ssize_t;
        #elif __SIZEOF_POINTER__ == 8
              typedef  u64   size_t;
              typedef  i64  ssize_t;
        #else
            #error "size_t is neither 4 bytes nor 8 bytes...what?"
        #endif
    #endif

    #define nullptr ((void*)(0))

    // casting
    #define cast(variable, type) ( (type)(variable) )

    #define u64c(variable)  cast((variable), u64)
    #define u32c(variable)  cast((variable), u32)
    #define u16c(variable)  cast((variable), u16)
    #define  u8c(variable)  cast((variable),  u8)

    #define i64c(variable)  cast((variable), i64)
    #define i32c(variable)  cast((variable), i32)
    #define i16c(variable)  cast((variable), i16)
    #define  i8c(variable)  cast((variable),  i8)

    #define u64ptr(variable)  cast((variable), u64*)
    #define u32ptr(variable)  cast((variable), u32*)
    #define u16ptr(variable)  cast((variable), u16*)
    #define  u8ptr(variable)  cast((variable),  u8*)

    #define i64ptr(variable)  cast((variable), i64*)
    #define i32ptr(variable)  cast((variable), i32*)
    #define i16ptr(variable)  cast((variable), i16*)
    #define  i8ptr(variable)  cast((variable),  i8*)

    #define voidptr(variable) cast ((variable), void*)
    #define voidval cast(0, void) // stops compiler from complaining

    #define castcall(rettype, fname, ...) (*(rettype (*)(__VA_ARGS__)) fname)

    #define dref(addr)    (*(addr))
    #define in(addr)    dref(addr)

    #define addr(var)     (&(var))
    #define at(var)       addr(var)
    #define from(var)     addr(var)
    #define of(var)       addr(var)
    #define to(var)       addr(var)

    #define MAX8  256
    #define MAX16 (MAX8  << 8)
    #define MAX32 (MAX16 << 8)
    #define MAX64 (MAX32 << 8)

    #define SIZECHECKER(structure, size) static char p__LINE__[ (sizeof(structure) == (size)) ? 1 : -1]

#endif // TYPES_H
