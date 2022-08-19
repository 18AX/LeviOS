#ifndef HPET_HEADER
#define HPET_HEADER

#include <levi/init.h>

#define HPET_GENERAL_CAPABILITIES 0x0
#define HPET_GENERAL_CONFIGURATION 0x10
#define HPET_GENERAL_INTERRUPTS_STATUS 0X20
#define HPET_MAIN_COUNTER 0xF0
#define HPET_TIMER_CONF_CAPABILITIES(N)                                        \
    ((0x100 + 0x20 * N) - (0x107 + 0x20 * N))
#define HPET_TIMER_COMPARATOR_VALUE(N) ((0x108 + 0x20 * N) - (0x10F + 0x20 * N))
#define HPET_TIMER_FSB_INTERRUPTS_ROUTE(N)                                     \
    ((0x110 + 0x20 * N) - (0x117 + 0x20 * N))

#define HPET_SET_TIMER_INTERRUPTS_ACTIVE(N) (1 << N);

struct hpet_general_capabilites
{
    union
    {
        struct
        {
            u8 rev_id;
            u8 amount_of_timer : 5;
            u8 count_size_cap : 1;
            u8 reserved : 1;
            u8 legacy_capability : 1;
            u16 vendor_id;
            u32 counter_clk_period;
        } __attribute__((packed));

        struct
        {
            u64 raw;
        } __attribute__((packed));
    };

} __attribute__((packed));

struct hpet_general_configuration
{
    union
    {
        struct
        {
            u8 enable_cnf : 1;
            u8 enbale_legacy_mapping : 1;
            u64 reserved : 62;
        } __attribute__((packed));

        struct
        {
            u64 raw;
        } __attribute__((packed));
    };
} __attribute((packed));

struct hpet_general_interrupt
{
    union
    {
        struct
        {
            u32 timers_interrupt_active;
            u32 reserved;
        } __attribute__((packed));

        struct
        {
            u64 raw;
        } __attribute__((packed));
    };
} __attribute__((packed));

struct hpet_timer_conf_cap
{
    union
    {
        struct
        {
            u8 reserved0 : 1;
            u8 interrupts_type : 1; /** edge or level trigger **/
            u8 enable_trigger_interrupts : 1;
            u8 timer_type : 1; /** Periodic or one shot **/
            u8 size_cap : 1;
            u8 reserved1 : 1;
            u8 value_set : 1;
            u8 force_32bit : 1;
            u8 interrupts_route : 5;
            u8 fsb_enable : 1;
            u8 fsb_cap : 1; /** rd only **/
            u16 reserved2;
            u32 interrupts_route_cap; /** Set the irq available for ioapic **/
        } __attribute__((packed));

        struct
        {
            u64 raw;
        } __attribute__((packed));
    };
} __attribute__((packed));

STATUS
hpet_init(void);

u64 hpet_read(u64 reg);

void hpet_write(u64 reg, u64 value);

void hpet_reset(void);

void hpet_wait(u64 ticks);

u64 hpet_ticks_from_ms(u64 ms);

#endif