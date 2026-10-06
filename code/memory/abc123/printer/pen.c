#include "dep/pen.h"
#include "dep/arena.h"
#include "dep/phase.h"

u64 pen_unsheath()
{
    return arena_map(PAGES(1)); // returns an arena block descriptor
}

u64 pen_col(u8* color, u64 paper)
{
    u64 amt = (color != clear) ? COLOR_STR_SIZE
                               : CLEAR_STR_SIZE;

    return arena_amt_avail(amt, paper) ? pen_buf(color, amt, paper)
                                       : paper;
}

u64 pen_buf(u8* ballpoint, u64 thoughts, u64 paper)
{
    u64 amtrecorded = arena_fill(ballpoint, thoughts, at(paper), ARENA_SHOULD_NOT_GROW);

    return (amtrecorded == thoughts) ? paper
                                     : arena_siphon(amtrecorded, paper);
}

u64 pen_dec(u64 value, u64 paper)
{
    u64 amt;

    u8 buffer[PHASE_DEC_BUFFER_SIZE];

    amt = num2decstr(value, buffer);

    return arena_amt_avail(amt, paper) ? pen_buf(buffer, amt, paper)
                                       : paper;
}

u64 pen_hex(u64 value, u64 paper)
{
    u64 amt;

    u8 buffer[PHASE_HEX_BUFFER_SIZE];

    amt = num2hexstr(value, buffer);

    return arena_amt_avail(amt, paper) ? pen_buf(buffer, amt, paper)
                                       : paper;
}


u64 pen_oct(u64 value, u64 paper)
{
    u64 amt;

    u8 buffer[PHASE_OCT_BUFFER_SIZE];

    amt = num2octstr(value, buffer);

    return arena_amt_avail(amt, paper) ? pen_buf(buffer, amt, paper)
                                       : paper;
}

u64 pen_bin(u64 value, u64 paper)
{
    u64 amt;

    u8 buffer[PHASE_BIN_BUFFER_SIZE];

    amt = num2binstr(value, buffer);

    return arena_amt_avail(amt, paper) ? pen_buf(buffer, amt, paper)
                                       : paper;
}

u64 pen_val(u64 value, u64 paper, u64 format)
{
    switch(format)
    {
        case PEN_DEC : return pen_dec(value, paper);

        case PEN_HEX : return pen_hex(value, paper);

        case PEN_BIN : return pen_bin(value, paper);

        case PEN_OCT : return pen_oct(value, paper);
    }
}

u64 pen_raw8(u8 inject, u64 paper)
{
    return arena_amt_avail(1, paper) ? pen_buf(u8ptr(addr(inject)), 1, paper)
                                     : paper;
}

u64 pen_raw16(u16 inject, u64 paper)
{
    return arena_amt_avail(2, paper) ? pen_buf(u8ptr(addr(inject)), 2, paper)
                                     : paper;
}

u64 pen_raw32(u32 inject, u64 paper)
{
    return arena_amt_avail(4, paper) ? pen_buf(u8ptr(addr(inject)), 4, paper)
                                     : paper;
}

u64 pen_raw64(u64 inject, u64 paper)
{
    return arena_amt_avail(8, paper) ? pen_buf(u8ptr(addr(inject)), 8, paper)
                                     : paper;
}

u64 pen_write(u64 descriptor, u64 block)
{
    return arena_flush(descriptor, block);
}

u64 pen_sheath(u64 paper)
{
    return arena_unmap(paper);
}
