#ifndef REVERSE_H
#define REVERSE_H

#include "dep/unite.h"

#define SKIP_GROUPS_OF_4 0b100
#define SKIP_GROUPS_OF_2 0b010
#define SKIP_BITS        0b001
#define NO_SKIP          0

 u8 reverse_byte(u8 byte, u8 skip);
u16 reverse_2bytes(u16 num, u8 skip);
u32 reverse_4bytes(u32 num, u8 skip);
u64 reverse_8bytes(u64 num, u8 skip);

#endif // REVERSE_H
