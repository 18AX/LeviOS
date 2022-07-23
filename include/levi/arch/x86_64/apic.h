#ifndef APIC_HEADER
#define APIC_HEADER

#include <levi/types.h>

STATUS apic_init(void);

void lapic_write(u32 reg, u32 data);

u32 lapic_read(u32 reg);

#endif