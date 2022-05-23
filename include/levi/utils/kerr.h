#ifndef KERR_HEADER
#define KERR_HEADER

#include <levi/types.h>
#include <stdarg.h>

#define KERROR_NOT_FOUND 0x1

void kerr(u32 code, const char *fmt, ...);

#endif