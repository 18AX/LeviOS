#include <levi/arch/x86_64/cmos.h>
#include <levi/arch/x86_64/io.h>

static u8 read_reg(u16 reg)
{
    outb(CMOS_ADDRESS, reg);

    return inb(CMOS_DATA);
}

static u32 is_updating(void)
{
    return (read_reg(CMOS_REG_A) & 0x80) != 0;
}

void cmos_get_time(struct time *time)
{
    while (is_updating() == 1)
        ;

    time->seconds = read_reg(CMOS_SECONDS_REG);
    time->minutes = read_reg(CMOS_MINUTES_REG);
    time->hours = read_reg(CMOS_HOURS_REG);
    time->weekday = read_reg(CMOS_WEEKDAY_REG);
    time->day = read_reg(CMOS_DAY_REG);
    time->month = read_reg(CMOS_MONTH_REG);
    time->year = read_reg(CMOS_YEAR_REG);
}