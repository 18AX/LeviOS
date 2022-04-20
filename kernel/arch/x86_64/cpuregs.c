#include <levi/arch/x86_64/cpuregs.h>

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