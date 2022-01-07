#include "levi/drivers/drivers.h"

#include "levi/drivers/serial/serial.h"

STATUS drivers_init()
{
    if (serial_init() == FAILED)
    {
        return FAILED;
    }

    return SUCCESS;
}