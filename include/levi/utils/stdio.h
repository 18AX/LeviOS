#ifndef STDIO_HEADER
#define STDIO_HEADER

#include <stdarg.h>

int vsprintf(char *buf, const char *fmt, va_list args);

int sprintf(char *buf, const char *fmt, ...);

int printf(const char *fmt, ...);

int puts(const char *s);

#endif