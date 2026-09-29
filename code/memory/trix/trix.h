#ifndef TRIX_H
#define TRIX_H

#include "dep/unite.h"

void    memcpy(void* dest, void* src, u64 len);
  u8     memeq(void* dest, void* src, u64 len);
void    memset(void* dest, u64 c, u64 len);
 u64 memtilnul(void* ptr);
void    memxor(void* dest, void* src, u64 len);

#define memzero(dest, len) memset((dest), 0x0000000000000000, (len))

// TODO
// this library is not compiled with math.a provided in the gcc command (FIX THIS)

#endif // TRIX_H
