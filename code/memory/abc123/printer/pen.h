#ifndef SCRIBE_H
#define SCRIBE_H


#include "dep/unite.h"

#define PEN_HEX 0x10
#define PEN_DEC   10
#define PEN_OCT 0010
#define PEN_BIN 0b10
#define PEN_NO_FLUSH NULL_DESCRIPTOR

#define pen_block(pen) arena_block(pen)
#define pen_chr(chr, block) pen_raw8((chr), (block))

u64 pen_col(u8* ptr, u64 paper);
u64 pen_buf(u8* ballpoint, u64 thoughts, u64 paper);

u64 pen_dec(u64 value, u64 paper);
u64 pen_hex(u64 value, u64 paper);
u64 pen_oct(u64 value, u64 paper);
u64 pen_bin(u64 value, u64 paper);
u64 pen_val(u64 value, u64 paper, u64 format);

u64 pen_raw8(u8 inject, u64 paper);
u64 pen_raw16(u16 inject, u64 paper);
u64 pen_raw32(u32 inject, u64 paper);
u64 pen_raw64(u64 inject, u64 paper);

u64 pen_unsheath();
u64 pen_write(u64 descriptor, u64 block);
u64 pen_sheath(u64 paper);


#endif // SCRIBE_H
