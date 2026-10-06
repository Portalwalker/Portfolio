#ifndef CTYPE_H
#define CTYPE_H

#include "dep/unite.h"

u8 it_is_upper(u8 c);
u8 it_is_lower(u8 c);
u8 it_is_hex(u8 c);
u8 it_is_alpha(u8 c);
u8 it_is_digit(u8 c);
u8 it_is_alnum(u8 c);
u8 it_is_space(u8 c);
u8 toupper(u8 c);
u8 tolower(u8 c);

#endif // CTYPE_H 
