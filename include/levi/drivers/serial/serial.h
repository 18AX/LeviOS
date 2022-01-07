#ifndef SERIAL_DRIVER
#define SERIAL_DRIVER

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

#include "levi/types.h"

STATUS serial_init(void);

#endif