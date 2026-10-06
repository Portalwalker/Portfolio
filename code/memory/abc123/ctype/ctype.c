#include "dep/ctype.h"

/*********************************************************** 
 * c : ascii character
 * 
 * desc 
 *       determines if character matches a category of ascii characters
 *
 * <--- returns true/false
 ***********************************************************/

u8 it_is_upper(u8 c)
{
    return (u8c(c) >= u8c('A') && u8c(c) <= u8c('Z'));
}
u8 it_is_lower(u8 c)
{
    return (u8c(c) >= u8c('a') && u8c(c) <= u8c('z'));
}
u8 it_is_hex(u8 c)
{
    return ((u8c(c) >= u8c('a') && u8c(c) <= u8c('f')) ||
            (u8c(c) >= u8c('A') && u8c(c) <= u8c('F')));
}
u8 it_is_alpha(u8 c)
{
    return (it_is_lower(c) || 
            it_is_upper(c));
}
u8 it_is_digit(u8 c)
{
    return (u8c(c) >= u8c('0') && u8c(c) <= u8c('9'));
}
u8 it_is_alnum(u8 c)
{
    return (it_is_alpha(c) || 
            it_is_digit(c));
}
u8 it_is_space(u8 c)
{
    return (u8c(c) == u8c(' ') || (u8c(c) >= u8c('\t') && u8c(c) <= u8c('\r')));
}

/***********************************************************
 * c : ascii character
 * 
 * desc 
 *      sets the casing of alphabetic ascii characters
 *
 * <--- returns the correctly cased character 
 *              or 'c' if character was not alphabetic
 ***********************************************************/

u8 toupper(u8 c)
{
    return (it_is_lower(c)) ? u8c(u8c(c) & u8c(0x5f)) : c;
}
u8 tolower(u8 c)
{
    return (it_is_upper(c)) ? u8c(u8c(c) | u8c(0x20)) : c;
}
