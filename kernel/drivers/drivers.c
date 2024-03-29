#include <levi/drivers/console/console.h>
#include <levi/drivers/drivers.h>
#include <levi/drivers/framebuffer/framebuffer.h>
#include <levi/drivers/kbd/intel8042.h>
#include <levi/drivers/serial/serial.h>

STATUS drivers_init(struct stivale2_struct *boot_info)
{
#if x86_64
    if (serial_init() == FAILED)
    {
        return FAILED;
    }

    if (intel8042_init() == FAILED)
    {
        return FAILED;
    }
#endif

    if (framebuffer_init(boot_info) == FAILED)
    {
        return FAILED;
    }

    if (console_init() == FAILED)
    {
        return FAILED;
    }

    return SUCCESS;
}