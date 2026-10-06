#ifndef TRIX_H
#define TRIX_H

#include "dep/unite.h"
#include "dep/reverse.h"

 u64 count_til_nul(void* ptr);

void    memcpy(void* dest, void* src, u64 len);
  u8     memeq(void* dest, void* src, u64 len);
void    memset(void* dest, u64 c, u64 len);
void    memxor(void* dest, void* src, u64 len);
void    memrev(void* src, u64 len, u64 skipflags);

#define mem_reverse_bits(dest, len)  memrev((dest), (len), NO_SKIP)
#define mem_reverse_pairs(dest, len) memrev((dest), (len), SKIP_GROUPS_OF_2)
#define mem_reverse_quads(dest, len) memrev((dest), (len), SKIP_GROUPS_OF_4)
#define mem_reverse(dest, len)       memrev((dest), (len), SKIP_BITS)

#define memzero(dest, len) memset((dest), 0x0000000000000000, (len))

// TODO
// this library is not compiled with math.a provided in the gcc command (FIX THIS)

#endif // TRIX_H
