#ifndef ENDIAN_H
#define ENDIAN_H

#include "dep/unite.h"

// Byte Index:   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
// ------------------------------------------------------------
// ASCII:       45 78 61 6d 70 6c 65
// UTF-16BE:    FE FF 00 45 00 78 00 61 00 6d 00 70 00 6c 00 65
// UTF-16LE:    FF FE 45 00 78 00 61 00 6d 00 70 00 6c 00 65 00

// ASCII    : 0x--45   78   61   6d   70   6c   65
// UTF-16BE : 0x4500 7800 6100 6d00 7000 6c00 6500
// UTF-16LE : 0x0045 0078 0061 006d 0070 006c 0065

#define ENDIAN_LITTLE 0x0F
#define ENDIAN_BIG    0xF0
#define ENDIAN_VAGUE  0x00

u16 atowLE(u8 byte);
u16 atowBE(u8 byte);
 u8 wLEtoa(u16 word);
 u8 wBEtoa(u16 word);
u16 wEtowE(u16 word);
u32 iEtoiE(u32 dword);
u64 lEtolE(u64 qword);

u8 detect_endian_2words(u16 word1, u16 word2);
u8 detect_endian_1word(u16 word);

u8 isLE();

#endif // ENDIAN_H
