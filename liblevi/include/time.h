#ifndef TIME_HEADER
#define TIME_HEADER

#include <types.h>

struct time
{
    u32 seconds;
    u32 minutes;
    u32 hours;
    u32 weekday;
    u32 day;
    u32 month;
    u32 year;
};

#endif