#ifndef KERR_HEADER
#define KERR_HEADER

#include <levi/types.h>
#include <stdarg.h>

#define KERROR_UNKNOW 0x1
#define KERROR_NOT_FOUND 0x2
#define KERROR_UNEXPECTED_INTERRUPTION 0x3
#define KERROR_VIRTUAL_MEMORY 0x4

void kerr(u32 code, const char *fmt, ...);

#endif