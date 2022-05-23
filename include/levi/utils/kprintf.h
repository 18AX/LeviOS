#ifndef KPRINTF_HEADER
#define KPRINTF_HEADER

#include <stdarg.h>

int kvsprintf(char *buf, const char *fmt, va_list args);

int ksprintf(char *buf, const char *fmt, ...);

int __kdprintf(s32 fd, const char *fmt, va_list args);

int kdprintf(s32 fd, const char *fmt, ...);

int kprintf(const char *fmt, ...);

int kputs(const char *s);

#endif