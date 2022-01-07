#ifndef STRING_HEADER
#define STRING_HEADER

#include "levi/types.h"

void *memcpy(void *dest, const void *src, u64 n);

u64 strlen(const char *str);

u64 strnlen(const char *s, u64 maxlen);

s32 strncmp(const char *s1, const char *s2, u32 n);

char *strncpy(char *destination, const char *source, u64 num);

s32 strcmp(const char *s1, const char *s2);

STATUS split(const char *str, char delimiter, char *output[]);

#endif