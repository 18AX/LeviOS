#include <levi/memory/memory.h>

static vas_t kernel_vas = { NULL };

vas_t kvas()
{
    return kernel_vas;
}

STATUS memory_init(vas_t *kvas)
{
    if (vmmap_range(kvas, 0x0, HHDM_ADDRESS, GIB_4, VM_READ_WRITE)
        == MAP_FAILED)
    {
        return FAILED;
    }

    if (vmmap_range(kvas, 0x0, KERNEL_SPACE, KERNEL_SPACE_SIZE, VM_READ_WRITE)
        == MAP_FAILED)
    {
        return FAILED;
    }

    kernel_vas = *kvas;

    return switch_vas(kvas);
}