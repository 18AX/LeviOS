#include <levi/arch/x86_64/acpi.h>
#include <levi/arch/x86_64/hpet.h>
#include <levi/memory/memory.h>

static volatile u8 *hpet_ptr = NULL;
static u64 hpet_period = 0;
static u64 hpet_min_ticks = 0;
static struct hpet_general_configuration hpet_general_conf = { 0 };

#include <levi/utils/kprintf.h>

STATUS hpet_init()
{
    struct hpet_header *header = (struct hpet_header *)sdt_find(SDT_SIG_HPET);

    /** address_space_id = 1 means is System IO **/
    if (header == NULL || header->address_space_id == 1)
    {
        return FAILED;
    }

    hpet_ptr = (volatile u8 *)phys_to_hhdm(header->address);
    hpet_min_ticks = header->minimum_tick;

    u64 raw = hpet_read(HPET_GENERAL_CAPABILITIES);

    struct hpet_general_capabilites general_cap =
        (struct hpet_general_capabilites){ .raw = raw };

    /** freq = 10^15 / period **/
    hpet_period = general_cap.counter_clk_period;

    raw = hpet_read(HPET_GENERAL_CONFIGURATION);

    hpet_general_conf.raw = raw;

    kprintf("hpet freq: %ld\n", hpet_period);

    return SUCCESS;
}

u64 hpet_read(u64 reg)
{
    return *((volatile u64 *)&hpet_ptr[reg]);
}

void hpet_write(u64 reg, u64 value)
{
    *((volatile u64 *)&hpet_ptr[reg]) = value;
}

void hpet_reset()
{
    hpet_write(HPET_MAIN_COUNTER, 0);
}

void hpet_wait(u64 ticks)
{
    hpet_general_conf.enable_cnf = 1;
    hpet_write(HPET_GENERAL_CONFIGURATION, hpet_general_conf.raw);

    while (hpet_read(HPET_MAIN_COUNTER) < ticks)
        ;

    hpet_general_conf.enable_cnf = 0;
    hpet_write(HPET_GENERAL_CONFIGURATION, hpet_general_conf.raw);
}

u64 hpet_ticks_from_ms(u64 ms)
{
    /** 0xe8d4a51000 = 10^12 **/
    return (ms * 0xe8d4a51000) / hpet_period;
}