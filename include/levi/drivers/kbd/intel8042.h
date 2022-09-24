#ifndef INTEL8042_HEADER
#define INTEL8042_HEADER

#include <levi/types.h>

#define INTEL8042_READ 0x60
#define INTEL8042_STATUS 0x64

#define INTEL8042_IRQ 0x31
#define INTEL8042_QUEUE_SIZE 128

STATUS intel8042_init(void);

#endif