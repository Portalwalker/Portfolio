#include "dep/reverse.h"

u8 reverse_4bits(u8 halfbyte)
{
    static u8 reverse_table[16] = { 0b0000, 0b1000, 0b0100, 0b1100,
                                    0b0010, 0b1010, 0b0110, 0b1110,
                                    0b0001, 0b1001, 0b0101, 0b1101,
                                    0b0011, 0b1011, 0b0111, 0b1111 };

        return dref(reverse_table + halfbyte);
}

u8 reverse_byte(u8 byte, u8 skip)
{
    return (skip >= SKIP_BITS) ? byte : reverse_4bits(u8c((u8c(0b11110000) & byte) >> 4)) | (reverse_4bits(u8c(u8c(0b00001111) & byte)) << 4);
}

u16 reverse_2bytes(u16 num, u8 skip)
{
    return (skip >= SKIP_GROUPS_OF_2) ? u16c(reverse_byte(u8c(num), SKIP_BITS)) | u16c(reverse_byte(u8c(num >> 8), SKIP_BITS) << 8)
                                      : u16c(reverse_byte(u8c(num >> 8), skip)) | u16c(reverse_byte(u8c(num), skip) << 8);
}

u32 reverse_4bytes(u32 num, u8 skip)
{
    return (skip >= SKIP_GROUPS_OF_4) ? u32c(reverse_2bytes(u16c(num >> 16), SKIP_GROUPS_OF_2) << 16) | u32c(reverse_2bytes(u16c(num), SKIP_GROUPS_OF_2))
                                      : u32c(u32c(reverse_2bytes(u16c(num), skip)) << 16)             | u32c(reverse_2bytes(u16c(num >> 16), skip));
}

u64 reverse_8bytes(u64 num, u8 skip)
{
    return u64c(u64c(reverse_4bytes(u32c(num), skip)) << 32) | u64c(reverse_4bytes(u32c(num >> 32), skip));
}


