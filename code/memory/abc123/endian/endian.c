#include "dep/endian.h"

u16 atowLE(u8 byte)
{
    return u16c(0x0000) | u16c(byte);
}
u16 atowBE(u8 byte)
{
    return u16c(0x0000) | u16c(u16c(byte) << 8);
}
u8 wLEtoa(u16 word)
{
    return u8c(word);
}
u8 wBEtoa(u16 word)
{
    return u8c(u16c(word) >> 8);
}
u16 wEtowE(u16 word)
{
   return u16c(word << 8) | u16c(word >> 8);
}
u32 iEtoiE(u32 dword)
{  // [+] positions 1 & 4 and 2 & 3 swap
   // [+] goes from 32bit 0x01020304 to 
   //                     0x04030201
   return (u32c(wEtowE(u16c(dword))) << 16) | u32c(wEtowE(u16c(dword >> 16)));
}

u64 lEtolE(u64 qword)
{  // [+]  64 bit version of iEtoiE
   return (u64c(iEtoiE(u32c(qword))) << 32) | u64c(iEtoiE(u32c(qword >> 32)));
}

u8 detect_endian_2words(u16 word1, u16 word2)
{   
    // NOTE: ?? == non-zero
    
    // if 0x00?? 0x00?? --> little endian
    if ( (word1 & 0x00FF) && !(word1 & 0xFF00) &&
         (word2 & 0x00FF) && !(word2 & 0xFF00) )
    {
        return ENDIAN_LITTLE;
    }
    
    // if 0x??00 0x??00 --> big endian
    if ( !(word1 & 0x00FF) && (word1 & 0xFF00) &&
         !(word2 & 0x00FF) && (word2 & 0xFF00) )
    {
        return ENDIAN_BIG;
    }
    
    switch (word1)
    {
        case 0xFFFE : return ENDIAN_LITTLE;
        case 0xFEFF : return ENDIAN_BIG;
    }
    
    switch (word2)
    {
        case 0xFEFF : return ENDIAN_BIG;
    }
    
    // if 0x???? 0x???? OR 
    //    0x0000 0x0000
    return ENDIAN_VAGUE;
}
u8 detect_endian_1word(u16 word)
{   
    // NOTE: ?? == non-zero
    
    // if 0x00?? --> little endian
    if ( (word & 0x00FF) && !(word & 0xFF00))
    {
        return ENDIAN_LITTLE;
    }
    
    // if 0x??00 --> big endian
    if ( !(word & 0x00FF) && (word & 0xFF00))
    {
        return ENDIAN_BIG;
    }
    
    switch (word)
    {
        case 0xFFFE : return ENDIAN_LITTLE;
        case 0xFEFF : return ENDIAN_BIG;
    }
    
    // if 0x???? 0x???? OR 
    //    0x0000 0x0000
    return ENDIAN_VAGUE;
}

u8 isLE()
{
   u32 test = 0x01234567;
   return (dref(u8ptr(&test)) == 0x67);
}
