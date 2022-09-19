#ifndef CMOS_HEADER
#define CMOS_HEADER

#include <levi/time/time.h>

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

#define CMOS_SECONDS_REG 0x0
#define CMOS_MINUTES_REG 0x2
#define CMOS_HOURS_REG 0x4
#define CMOS_WEEKDAY_REG 0x6
#define CMOS_DAY_REG 0x7
#define CMOS_MONTH_REG 0x8
#define CMOS_YEAR_REG 0x9

#define CMOS_REG_A 0x0A
#define CMOS_REG_B 0x0B

void cmos_get_time(struct time *time);

#endif