#ifndef STDIO_HEADER
#define STDIO_HEADER

#define STDIN_FD 0
#define STDOUT_FD 1
#define STDERR_FD 2

#include <stdarg.h>
#include <types.h>

u64 snprintf(char *str, u32 size, const char *format, ...);

u64 vsnprintf(char *str, u32 size, const char *format, va_list arg);

u64 printf(const char *fmt, ...);

u64 puts(const char *str);

#endif