#ifndef PHASE_H
#define PHASE_H

#define PHASE_DEC_BUFFER_SIZE 20
#define PHASE_HEX_BUFFER_SIZE 16
#define PHASE_OCT_BUFFER_SIZE 22
#define PHASE_BIN_BUFFER_SIZE 64


#include "dep/unite.h"


u64 num2decstr(u64 value, u8* buffer);
u64 num2hexstr(u64 value, u8* buffer);
u64 num2octstr(u64 value, u8* buffer);
u64 num2binstr(u64 value, u8* buffer);

u64 decstr2num(u8* ptr, u64 len);
u64 hexstr2num(u8* ptr, u64 len);
u64 octstr2num(u8* ptr, u64 len);
u64 binstr2num(u8* ptr, u64 len);


#endif // PHASE_H
