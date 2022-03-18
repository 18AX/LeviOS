#include "levi/drivers/drivers.h"

#include "levi/drivers/serial/serial.h"

STATUS drivers_init()
{
#if x86_64
    if (serial_init() == FAILED)
    {
        return FAILED;
    }
#endif

    return SUCCESS;
}