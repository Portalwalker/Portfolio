#include "dep/phase.h"
#include "dep/ctype.h"
#include "dep/trix.h"


u64 num2binstr(u64 value, u8* buffer)
{
    u8 i;
    i = 0;
    do
    {
        dref(buffer + i) = '0' + ((value & u64c(0x8000000000000000)) != 0);
        value = value << 1;
        ++i;
    } while (i < PHASE_BIN_BUFFER_SIZE);

    return PHASE_BIN_BUFFER_SIZE;
}

u64 num2decstr(u64 value, u8* buffer)
{
    u8 i;

    i = 0;
    do
    {
        dref(buffer + i) = '0' + (u64c(value) % u64c(10));
        value = u64c(value) / u64c(10);
        ++i;
    } while (value);

    dref(buffer + i) = 0;

    mem_reverse(buffer, i);

    return i;
}

u64 num2hexstr(u64 value, u8* buffer)
{
    u8 c;
    u8 i;
    i = 0;
    do
    {
        c = u8c(u64c(value & u64c(0xf000000000000000)) >> 60);
        dref(buffer + i) = (c < 0xa) ? '0' + (c) :
        'A' + (c - 0xa);
        value = value << 4;
        ++i;
    } while (i < PHASE_HEX_BUFFER_SIZE);

    return PHASE_HEX_BUFFER_SIZE;
}

u64 num2octstr(u64 value, u8* buffer)
{
    u8 i;

    dref(buffer) = '0' + u8c((u64c(value >> 63) & 1));
    value = value << 1;

    i = 1;
    do
    {
        dref(buffer + i) = '0' + u8c(u64c(value >> 61) & u64c(07));
        value = value << 3;
        ++i;
    } while (i < PHASE_OCT_BUFFER_SIZE);

    return PHASE_OCT_BUFFER_SIZE;
}

u64 decstr2num(u8* ptr, u64 len)
{
    u64 num = 0;

    len = (len <= PHASE_DEC_BUFFER_SIZE) ? len
                                         : PHASE_DEC_BUFFER_SIZE;

    while (len > 0)
    {
        --len;
        if (!it_is_digit(dref(ptr)))
        {
            break;
        }
        num = u64c(u64c(num << 3) + u64c(num << 1) + u64c(dref(ptr) - '0'));
        ++ptr;
    }

    return num;
}

u64 hexstr2num(u8* ptr, u64 len)
{
    u64 num = 0;
    u8 tempval;

    len = (len <= PHASE_HEX_BUFFER_SIZE) ? len
                                         : PHASE_HEX_BUFFER_SIZE;

    while (len > 0)
    {
        --len;
                         tempval = dref(ptr);
        if (!it_is_digit(tempval))
        {
            tempval = toupper(tempval);

            if ( !(tempval  >=  'A'  &&  tempval  <=  'F') )
            {
                break;
            }
        }

        num = u64c(u64c(num << 4) | u64c(tempval - ((tempval  >=  'A') ? ('A' - 10) : '0')));

        ++ptr;
    }

    return num;
}

u64 octstr2num(u8* ptr, u64 len)
{
    u64 num = 0;

    len = (len <= PHASE_OCT_BUFFER_SIZE) ? len
                                         : PHASE_OCT_BUFFER_SIZE;

    while (len > 0)
    {
        --len;
        if (!(dref(ptr) >= '0' && dref(ptr) <= '7'))
        {
            break;
        }
        num = u64c(u64c(num << 3) | u64c(dref(ptr) - '0'));
        ++ptr;
    }

    return num;
}


u64 binstr2num(u8* ptr, u64 len)
{
    u64 num = 0;

    len = (len <= PHASE_BIN_BUFFER_SIZE) ? len
    : PHASE_BIN_BUFFER_SIZE;

    while (len > 0)
    {
        --len;
        if (dref(ptr) != '0' && dref(ptr) != '1')
        {
            break;
        }
        num = u64c(u64c(num << 1) | u64c(dref(ptr) == '1'));
        ++ptr;
    }

    return num;
}
