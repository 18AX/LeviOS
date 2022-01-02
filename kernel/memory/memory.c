#include "levi/memory/memory.h"

static vas_t kernel_vas = { NULL };

vas_t kvas()
{
    return kernel_vas;
}

STATUS memory_init(vas_t *kvas)
{
    if (vmmap_range(kvas, 0x0, 0x0, GIB_4, VM_READ_WRITE) == MAP_FAILED)
    {
        return FAILED;
    }

    if (vmmap_range(kvas, 0x0, KERNEL_ADDRESS, KERNEL_MAP_SIZE, VM_READ_WRITE)
        == MAP_FAILED)
    {
        return FAILED;
    }

    kernel_vas = *kvas;

    return switch_vas(kvas);
}

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