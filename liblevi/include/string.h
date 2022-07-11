#ifndef STRING_HEADER
#define STRING_HEADER

#include <types.h>

void *memcpy(void *dest, const void *src, u64 n);

void *memset(void *dest, int val, u64 len);

u64 strlen(const char *str);

u64 strnlen(const char *s, u64 maxlen);

s32 strncmp(const char *s1, const char *s2, u32 n);

char *strncpy(char *destination, const char *source, u64 num);

char *strcpy(char *destination, const char *source);

s32 strcmp(const char *s1, const char *s2);

char *strchr(char *str, char c);

#endif