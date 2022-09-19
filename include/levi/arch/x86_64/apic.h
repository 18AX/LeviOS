#ifndef APIC_HEADER
#define APIC_HEADER

#include <levi/arch/x86_64/acpi.h>
#include <levi/types.h>

#define IOAPIC_TRIGGER_LOW (1)
#define IOAPIC_TRIGGER_HIGH (1 << 1)
#define IOAPIC_TRIGGER_EDGE (1 << 2)
#define IOAPIC_TRIGGER_LEVEL (1 << 3)
#define IOAPIC_INTERRUPT_MASK_SET (1 << 4)

#define CPUID_LAPIC_FEATURE (1 << 9)
#define MSR_APIC_BASE_ENABLE (1 << 11)
#define SPURIOUS_VECTOR_APIC_ENABLE (1 << 8)
#define SPURIOUS_VECTOR_NO_EOI_BROADCAST (1 << 12)

#define IO_REDIRECTION_TABLE_BASE 0x10
#define IO_REDIRECTION_TABLE_SIZE 0x17

#define LAPIC_ID 0x20
#define LAPIC_VERSION 0x30
#define LAPIC_TASK_PRIORITY 0x80
#define LAPIC_ARBITRATION_PRIORITY 0x90
#define LAPIC_PROCESSOR_PRIORITY 0xA0
#define LAPIC_EOI 0xB0
#define LAPIC_REMOTE_READ 0xC0
#define LAPIC_LOGICAL_DESTINATION 0xD0
#define LAPIC_SPURIOUS_INTERRUPT_VECTOR 0xF0
#define LAPIC_LVT_CMCI 0x2F0
#define LAPIC_LVT_TIMER 0x320
#define LAPIC_LVT_THERMAL_SENSOR 0x330
#define LAPIC_LVT_PMC                                                          \
    0x340 /** LVT Performance Monitoring Counters Register                     \
           **/
#define LAPIC_LVT_LINT0 0x350
#define LAPIC_LVT_LINT1 0x360
#define LAPIC_LVT_ERROR 0x370
#define LAPIC_TIMER_INITIAL_COUNT 0x380
#define LAPIC_TIMER_CURRENT_COUNT 0x390
#define LAPIC_TIMER_DIVIDE_CONF 0x3E0

#define LAPIC_LVT_VECTOR(N) (N & 0xFF)
#define LAPIC_LVT_NMI (1 << 10)
#define LAPIC_LVT_DISABLE (1 << 16)
#define LAPIC_LVT_DELIVERY_PENDING (1 << 12)

#define LAPIC_LVT_TIMER_ONESHOT 0x0
#define LAPIC_LVT_TIMER_PERIODIC (1 << 17)

struct apic_redirection
{
    union
    {
        struct
        {
            u8 interrupt_vector;
            u8 delivery_mode : 3;
            u8 destination_mode : 1;
            u8 delivery_status : 1;
            u8 polarity : 1;
            u8 remote_irr : 1;
            u8 trigger_mode : 1;
            u8 interrupt_mask : 1;
            u64 reserved : 39;
            u8 destination_field;
        } __attribute__((packed));

        struct
        {
            u32 low;
            u32 high;
        } __attribute__((packed));
    };
} __attribute__((packed));

STATUS apic_init(void);

void apic_enable(void);

void lapic_write(u32 reg, u32 data);

u32 lapic_read(u32 reg);

void ioapic_write(u32 reg, u32 data);

u32 ioapic_read(u32 reg);

u32 lapic_cpu_count(void);

/**
 * @brief retrieve apic ID and apic processor ID of a processor.
 *
 * @param cpuid
 * @param res
 * @return STATUS
 */
STATUS lapic_cpu_info(u8 cpuid, struct madt_lapic_proc *res);

/**
 * @brief Enable an irq by creating a redirection to the specified cpu.
 *
 * @param cpuid
 * @param apic_pin
 * @param interrupts_vector
 * @param flags
 */
STATUS ioapic_irq_set(u8 cpuid, u8 apic_pin, u8 interrupts_vector, u32 flags);

void lapic_eoi(void);

STATUS lapic_timer_init(void);

void lapic_timer_set(u32 ms);

#endif