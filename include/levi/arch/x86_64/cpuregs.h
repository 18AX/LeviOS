#ifndef CPUREGS_HEADER
#define CPUREGS_HEADER

#include <levi/types.h>

u64 cr2_read(void);

void cr2_write(u64 value);

#endif