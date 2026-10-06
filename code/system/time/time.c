#include "dep/time.h"



// ___ timeff
// ___________ fakes the time (sort of an entropy generator with a hardcoded seed meant only as a fallback)

void timeff(timebox64* tbox)
{
    static u8 counter = 0; // init. only once

    if (!tbox->tv_nsec)
    {
        tbox->tv_nsec = 7;
    }

    tbox->tv_nsec *= (TIMEFF_START_NSEC_INCREASE_FACTOR + u64c(&tbox) + (counter += 0b1)     +
                                                                        (counter  & 0b111)   +
                                                                        (counter  & 0b11111) +
                                                                        (counter  & 0b1111111));

    tbox->tv_sec += (TIMEFF_START_SEC_INCREASE_LIKELYHOOD & tbox->tv_nsec);
}



// ___ time_2038_portable_solution
// ___________ what the names says
// ___________ extends support for 32 bit time on 32 bit systems by 68 years

u64 time_2038_portable_solution(timebox64* tbox)
{
    // NOTE: determines what syscall to use: (clock_gettime64 [time64]) OR
    //                                       (clock_gettime   [time32])

    // NOTE: determines what CLOCK_TYPE to use: CLOCK_MONOTONIC (only incremental / high resolution) OR
    //                                          CLOCK_REALTIME  (fallback as it is most likely defined as 0 across many past operating systems)

    // NOTE: RETURN is an OR'ed pair of values (upper 32 bits == clocktype | lower 32 bits syscalltype)
    // NOTE: ERROR == u64c(-1) and redundant type casts are for clarity

    /* PSEUDO CODE FOR UNDERSTANDING
     *
     *      if clock_gettime64 exists, call it from now on => 64 bit time supported, no need for overflow
     * else if clock_gettime only, call it from now on => 32 bit time only so calculate overflow for the next 68 years
     * else if clock_gettime only, call it from now on => 32 bit time only so calculate overflow for the next 68 years
     * else if clock_gettime64 exists BUT monotonic NOT supported so use CLOCK_REALTIME as a fallback, no need for overflow
     * else if clock_gettime only AND monotonic NOT supported so use CLOCK_REALTIME as a fallback, calculate overflow for the next 68 years
     * else ALL HANDS ON DECK-- WE GOTTA FAKE THE CLOCK ENTROPY
     */

    if (u64c(time64(CLOCK_MONOTONIC, cast(tbox, timebox64*))) != ERROR)
    {
        tbox->tv_over = 0;

        return SET_UPPER_32_BITS(CLOCK_MONOTONIC) |
               SET_LOWER_32_BITS(TIME64);
    }
    else if (u64c(time32(CLOCK_MONOTONIC, cast(tbox, timebox32*))) != ERROR)
    {
        tbox->tv_nsec = UPPER_32_BITS(tbox->tv_sec);
        tbox->tv_sec  = LOWER_32_BITS(tbox->tv_sec);

        if (TIME_SNAPSHOT_BEFORE_2038 > tbox->tv_sec)
        {
            tbox->tv_sec += TIME_OFFSET_POST_2038;
            tbox->tv_over = TIME_OVERFLOW_2038;
        }
        else
        {
            tbox->tv_over = 0;
        }

        return SET_UPPER_32_BITS(CLOCK_MONOTONIC) |
               SET_LOWER_32_BITS(TIME32);
    }
    else if (u64c(time64(CLOCK_REALTIME, cast(tbox, timebox64*))) != ERROR)
    {
        tbox->tv_over = 0;

        return SET_UPPER_32_BITS(CLOCK_REALTIME) |
               SET_LOWER_32_BITS(TIME64);
    }
    else if (u64c(time32(CLOCK_REALTIME, cast(tbox, timebox32*)) != ERROR))
    {
        tbox->tv_nsec = UPPER_32_BITS(tbox->tv_sec);
        tbox->tv_sec  = LOWER_32_BITS(tbox->tv_sec);

        if (TIME_SNAPSHOT_BEFORE_2038 > tbox->tv_sec)
        {
            tbox->tv_sec += TIME_OFFSET_POST_2038;
            tbox->tv_over = TIME_OVERFLOW_2038;
        }
        else
        {
            tbox->tv_over = 0;
        }

        return SET_UPPER_32_BITS(CLOCK_REALTIME) |
               SET_LOWER_32_BITS(TIME32);
    }

    tbox->tv_nsec = TIMEFF_START_SEC;
    tbox->tv_sec  = TIMEFF_START_NSEC;
    tbox->tv_over = 0;

    return SET_LOWER_32_BITS(TIMEFF);
}



// ___ time
// ___________ forces 32 bit clock_gettime calls into the 64 bit struct format
// ___________ posix timespec.tv_pad is renamed to 'tv_over' and it is used to trigger post-2038 time calculations

void time(timebox64* tbox)
{
    static u64 clockinfo = 0; // initialized once

    switch(LOWER_32_BITS(clockinfo))
    {
        case TIME64 : time64(UPPER_32_BITS(clockinfo), cast(tbox, timebox64*));
                      return;


        case TIME32 : time32(UPPER_32_BITS(clockinfo), cast(tbox, timebox32*));
                      tbox->tv_nsec = UPPER_32_BITS(tbox->tv_sec);
                      tbox->tv_sec  = LOWER_32_BITS(tbox->tv_sec) + ((tbox->tv_over == TIME_OVERFLOW_2038) ? TIME_OFFSET_POST_2038 : 0);
                      return;


        case TIMEFF : timeff(tbox);
                      return;


            default : clockinfo = time_2038_portable_solution(tbox); // executes once
                      return;
    }
}
