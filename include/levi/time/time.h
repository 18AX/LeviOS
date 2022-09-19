#ifndef TIME_HEADER
#define TIME_HEADER

#include <levi/types.h>

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

u64 timer_init(void);

u64 timer_count(void);

#endif