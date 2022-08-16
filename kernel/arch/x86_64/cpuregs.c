#include <levi/arch/x86_64/cpuregs.h>
#include <levi/utils/string.h>

u64 cr2_read(void)
{
    u64 value = 0;

    asm volatile("movq %%cr2,%0" : "=r"(value));

    return value;
}

void cr2_write(u64 value)
{
    asm volatile("movq %0,%%cr2\n" ::"a"((u64)value));
}

void cpuid(u32 eax, u32 *eax_res, u32 *ebx_res, u32 *ecx_res, u32 *edx_res)
{
    u32 a, b, c, d;
    asm volatile("cpuid" : "=a"(a), "=b"(b), "=c"(c), "=d"(d) : "a"(eax));

    *eax_res = a;
    *ebx_res = b;
    *ecx_res = c;
    *edx_res = d;
}

void cpuid_manufacturer_id(char output[CPUID_MANUFACTURER_ID_LEN])
{
    u32 eax, ebx, ecx, edx = 0;

    cpuid(0, &eax, &ebx, &ecx, &edx);

    memcpy(output, &ebx, sizeof(u32));
    memcpy(output + sizeof(u32), &edx, sizeof(u32));
    memcpy(output + 2 * sizeof(u32), &ecx, sizeof(u32));
    output[CPUID_MANUFACTURER_ID_LEN - 1] = '\0';
}