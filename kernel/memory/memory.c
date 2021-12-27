#include "levi/memory/memory.h"

u64 align_up(u64 ptr)
{
    return (ptr + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
}

u64 align_down(u64 ptr)
{
    return ptr & ~(PAGE_SIZE - 1);
}

u32 is_align(u64 ptr)
{
    return (ptr & (PAGE_SIZE - 1)) == 0;
}

u64 kaddress_to_phys(u64 addr)
{
    return addr - KERNEL_ADDRESS;
}

u64 phys_to_kaddress(u64 addr)
{
    return addr + KERNEL_ADDRESS;
}

u64 hhdm_to_phys(u64 addr)
{
    return addr - HHDM_ADDRESS;
}

u64 phys_to_hhdm(u64 addr)
{
    return addr + HHDM_ADDRESS;
}