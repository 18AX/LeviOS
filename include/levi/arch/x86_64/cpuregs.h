#ifndef CPUREGS_HEADER
#define CPUREGS_HEADER

#include <levi/types.h>

#define CPUID_MANUFACTURER_ID_LEN 13

u64 cr2_read(void);

void cr2_write(u64 value);

void cpuid(u32 eax, u32 *eax_res, u32 *ebx_res, u32 *ecx_res, u32 *edx_res);

void cpuid_manufacturer_id(char output[CPUID_MANUFACTURER_ID_LEN]);

#endif