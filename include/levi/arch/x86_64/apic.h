#ifndef APIC_HEADER
#define APIC_HEADER

#include <levi/arch/x86_64/acpi.h>
#include <levi/types.h>

#define CPUID_LAPIC_FEATURE (1 << 9)
#define MSR_APIC_BASE_ENABLE (1 << 11)
#define SPURIOUS_VECTOR_APIC_ENABLE (1 << 8)

#define LAPIC_ID 0x20
#define LAPIC_VERSION 0x30
#define LAPIC_TASK_PRIORITY 0x80
#define LAPIC_ARBITRATION_PRIORITY 0x90
#define LAPIC_PROCESSOR_PRIORITY 0xA0
#define LAPIC_EOI 0xB0
#define LAPIC_REMOTE_READ 0xC0
#define LAPIC_LOGICAL_DESTINATION 0xD0
#define LAPIC_SPURIOUS_INTERRUPT_VECTOR 0xF0
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

void lapic_eoi(void);

#endif