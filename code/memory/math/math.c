#include "dep/math.h"

u64 absval(i64 val)
{
    return u64c(  (val < 0) ? -val : val  );
}

i64 maxval(i64 val1, i64 val2)
{
    return (val1 > val2) ? val1
                         : val2;
}

i64 minval(i64 val1, i64 val2)
{
    return (val1 < val2) ? val1
                         : val2;
}

u64 sum64_wrap_add_overflow(u64 val1, u64 val2)
{
    val1 += val2;
    val1 += (val1 < val2); // detects overflow
    return val1;
}
u32 sum32_wrap_add_overflow(u32 val1, u32 val2)
{
    val1 += val2;
    val1 += (val1 < val2);
    return val1;
}
u16 sum16_wrap_add_overflow(u16 val1, u16 val2)
{
    val1 += val2;
    val1 += (val1 < val2);
    return val1;
}
u16 sum16from64(u64 val)
{
    u32 calc32 = sum32_wrap_add_overflow(  u32c(u64c(val) >> 32),
                                           u32c(     val       )  );

    return sum16_wrap_add_overflow(  u16c(u32c(calc32) >> 16),
                                     u16c(     calc32       )  );
}
u16 sum16from32(u32 val)
{
    return sum16_wrap_add_overflow(  u16c(u32c(val) >> 16),
                                     u16c(     val       )  );
}
