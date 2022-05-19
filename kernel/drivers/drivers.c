#include <levi/drivers/drivers.h>
#include <levi/drivers/framebuffer/framebuffer.h>
#include <levi/drivers/serial/serial.h>

STATUS drivers_init(struct stivale2_struct *boot_info)
{
#if x86_64
    if (serial_init() == FAILED)
    {
        return FAILED;
    }
#endif

    if (framebuffer_init(boot_info) == FAILED)
    {
        return FAILED;
    }

    return SUCCESS;
}