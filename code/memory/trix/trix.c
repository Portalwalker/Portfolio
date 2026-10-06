#include "dep/trix.h"
#include "dep/math.h"
#include "dep/endian.h"

// ___ memcpy
// ___________ self explanatory

void memcpy(void* dest, void* src, u64 len)
{
    u64 remainder = len & u64c(0b111);
    len = len >> 3;

    while (len)
    {
        dref(u64ptr(dest)) = dref(u64ptr(src));
        dest = u8ptr(dest) + 8;
        src = u8ptr(src) + 8;
        --len;
    }
    while (remainder)
    {
        dref(u8ptr(dest)) = dref(u8ptr(src));
        dest = u8ptr(dest) + 1;
        src = u8ptr(src) + 1;
        --remainder;
    }
}



// ___ memeq
// ___________ if both memory sections are identical returns 1

u8 memeq(void* dest, void* src, u64 len)
{
    u64 remainder = len & u64c(0b111);
    len = len >> 3;

    while (len)
    {
        if (dref(u64ptr(dest)) != dref(u64ptr(src)))
        {
            return FALSE;
        }
        dest = u8ptr(dest) + 8;
        src = u8ptr(src) + 8;
        --len;
    }
    while (remainder)
    {
        if (dref(u8ptr(dest)) != dref(u8ptr(src)))
        {
            return FALSE;
        }
        dest = u8ptr(dest) + 1;
        src = u8ptr(src) + 1;
        --remainder;
    }
    return TRUE;
}



// ___ memset
// ___________ self explanatory

void memset(void* dest, u64 c, u64 len)
{
    u64 remainder = len & u64c(0b111);
    len = len >> 3;

    while (len)
    {
        dref(u64ptr(dest)) = c;
        dest = u8ptr(dest) + 8;
        --len;
    }
    while (remainder)
    {
        dref(u8ptr(dest)) = u8c(c);
        dest = u8ptr(dest) + 1;
        --remainder;
    }
}



// ___ memtilnul
// ___________ counts bytes of memory until a nul-terminator ('\0') is found
// ___________ mainly useful for initial strings handled main at the beginning of a program

u64 memtilnul(void* ptr)
{
    void* origin = ptr;
    while (*u8ptr(ptr))
    {
        ptr = u8ptr(ptr) + 1;
    }
    return absval(u64c(ptr) - u64c(origin));
}




// ___ memsum
// __________ checksums
// __________ adds extra zero byte if 'len' is odd

u16 memsum(void* ptr, u64 len)
{
    u64 calc64 = 0;
    u32 calc32 = 0;
    u16 sum = 0;
    u16 rem;
    // *remainder of 'len'

    rem = len & u64c(0b1111);
    len = len >> 4; // 'rem' needs value in 'len' before this line

    while (len)
    {
        calc64 = sum64_wrap_add_overflow(calc64, sum64_wrap_add_overflow(  dref(u64ptr(ptr) + 0),
                                                                           dref(u64ptr(ptr) + 1)  ));
        ptr = u8ptr(ptr) + 16;
        --len;
    }
    while (rem)
    {
        if (rem == 1)
        {
            sum = sum16_wrap_add_overflow(  sum,
                                            dref(u8ptr(ptr)) | 0x0000  );
            break;
        }

        sum = sum16_wrap_add_overflow(  sum,
                                        dref(u16ptr(ptr))  );

        ptr = u8ptr(ptr) + 2;
        rem -= 2;
    }

    return sum16_wrap_add_overflow(  sum,
                                     sum16from64( calc64 )  );
}



// ___ memxor
// __________ self explanatory

void memxor(void* dest, void* src, u64 len)
{
    u64 remainder = len & u64c(0b111);
    len = len >> 3;

    while (len)
    {
        dref(u64ptr(dest)) ^= dref(u64ptr(src));
        dest = u8ptr(dest) + 8;
        src = u8ptr(src) + 8;
        --len;
    }
    while (remainder)
    {
        dref(u8ptr(dest)) ^= dref(u8ptr(src));
        dest = u8ptr(dest) + 1;
        src = u8ptr(src) + 1;
        --remainder;
    }
}

// ___ memrev
// __________ reverses memory

void memrev(void* src, u64 len, u64 skipflags)
{
    u64 start;
    u64 end;
    u64 offset;
    u8 remstart;
    u8 remend;

    end = len >> 3;
    end -= (end != 0);
    start = 0;
    offset = len - sizeof(u64);

    remend = u8c(u64c(len) & u64c(0x0f));
    remend -= (remend != 0);
    remstart = 0;

    while (start < end)
    {
                dref(u64ptr(src) + start) = reverse_8bytes(dref(u64ptr(src) + start), skipflags);
        dref(u64ptr(u8ptr(src) + offset)) = reverse_8bytes(dref(u64ptr(u8ptr(src) + offset)), skipflags);

               dref(u64ptr(src) + start)  ^= dref(u64ptr(u8ptr(src) + offset));
        dref(u64ptr(u8ptr(src) + offset)) ^= dref(u64ptr(src) + start);
               dref(u64ptr(src) + start)  ^= dref(u64ptr(u8ptr(src) + offset));

        ++start;
        --end;
        offset -= sizeof(u64);
    }

    offset = (len >> 4) << 3;

    while (remstart < remend)
    {
        dref(u8ptr(src) + remstart + offset) = reverse_byte(dref(u8ptr(src) + remstart + offset), skipflags);
        dref(u8ptr(src) + remend   + offset) = reverse_byte(dref(u8ptr(src) + remend   + offset), skipflags);

        dref(u8ptr(src) + remstart + offset) ^= dref(u8ptr(src) + remend   + offset);
        dref(u8ptr(src) + remend   + offset) ^= dref(u8ptr(src) + remstart + offset);
        dref(u8ptr(src) + remstart + offset) ^= dref(u8ptr(src) + remend   + offset);

        ++remstart;
        --remend;
    }
}

