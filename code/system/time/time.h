#ifndef TIME_H
#define TIME_H



#include "dep/unite.h"



#define CLOCK_REALTIME 0 // NOTE: fallback clock
#if defined( OS_LINUX )
    #define CLOCK_MONOTONIC 1
#else
    #error "CLOCK: OS NOT SUPPORTED"
#endif



#define TIME_SNAPSHOT_BEFORE_2038   1789164273
#define TIME_OVERFLOW_2038          0xFFFFFFFF
#define TIME_OFFSET_POST_2038       0x100000000

#define TIMEFF_START_SEC   0x77777777
#define TIMEFF_START_NSEC  1799163134

#define TIMEFF_START_NSEC_INCREASE_FACTOR     0x77777777
#define TIMEFF_START_SEC_INCREASE_LIKELYHOOD  0b111

#define TIME32 32
#define TIME64 64
#define TIMEFF 0xFF // FAKE CLOCK FOR RANDOMNESS

#define TIME_WAVE_AMPLIFY 0x0000000099773311



typedef struct
{
    u32 tv_sec;
    u32 tv_nsec;
} timebox32;

typedef struct
{
    u64 tv_sec;
    u32 tv_nsec;
    u32 tv_over;
} timebox64;



void timeff(timebox64* tbox);
 u64 time_2038_portable_solution(timebox64* tbox);
void time(timebox64* tbox);



#endif // TIME_H
