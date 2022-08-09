#ifndef APIC_HEADER
#define APIC_HEADER

#include <levi/arch/x86_64/acpi.h>
#include <levi/types.h>

STATUS apic_init(void);

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
STATUS lapic_cpu_info(u8 cpuid, struct acpi_madt_local_apic_proc *res);

#endif