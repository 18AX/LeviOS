#ifndef MSR_HEADER
#define MSR_HEADER

#include <levi/types.h>

#define MSR_IA32_APIC_BASE 0x1B

static inline void __rdmsr(u32 msr, u32 *lo, u32 *hi)
{
    *lo = 0;
    *hi = 0;
    asm volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

static inline void __wrmsr(u32 msr, u32 low, u32 high)
{
    asm volatile("wrmsr" : : "a"(low), "d"(high), "c"(msr));
}

static inline u64 rdmsr64(u32 msr)
{
    u32 high = 0;
    u32 low = 0;

    __rdmsr(msr, &low, &high);

    return ((u64)low) | (((u64)high) << 32);
}

static inline void wrmsr64(u32 msr, u64 data)
{
    __wrmsr(msr, (u32)(data & 0xFFFFFFFF), (u32)((data >> 32) & 0xFFFFFFFF));
}

#endif