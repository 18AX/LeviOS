#ifndef INTEL8042_HEADER
#define INTEL8042_HEADER

#define INTEL8042_READ 0x60
#define INTEL8042_STATUS 0x64

void intel8042_init(void);

#endif