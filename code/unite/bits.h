#ifndef BITS_H
#define BITS_H

#include "dep/types.h"

#define BITMASK11 u64c(0b1111111111111111111111111111111111111111111111111111111111111111)
#define BITMASK00 (~(BITMASK11))

#define SET_UPPER_32_BITS(x) (u64c(x) << 32)
#define SET_LOWER_32_BITS(x) (u64c(x) & (BITMASK11 >> 32))

#define UPPER_32_BITS(x) (u64c(x) >> 32)
#define LOWER_32_BITS(x) (u64c(x) & (BITMASK11 >> 32))

#endif // BITS_H
