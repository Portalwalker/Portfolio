#ifndef MATH_H
#define MATH_H

#include "dep/unite.h"

u64 absval(i64);
i64 maxval(i64, i64);
i64 minval(i64, i64);

u64 sum64_wrap_add_overflow(u64, u64);
u32 sum32_wrap_add_overflow(u32, u32);
u16 sum16_wrap_add_overflow(u16, u16);
u16 sum16from64(u64);
u16 sum16from32(u32);

#endif // MATH_H
