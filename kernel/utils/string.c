#include "levi/utils/string.h"

void *memcpy(void *dest, const void *src, u64 n)
{
    const char *s = src;
    char *d = dest;

    for (u64 i = 0; i < n; i++)
        *d++ = *s++;

    return dest;
}

u64 strlen(const char *str)
{
    u64 len = 0;

    for (; str[len] != '\0'; ++len)
    {
    }

    return len;
}

u64 strnlen(const char *s, u64 maxlen)
{
    u64 i = 0;
    for (; i < maxlen; ++i)
        if (!s[i])
            return i;
    return i;
}

/**
 * This come from BSD
 */
s32 strncmp(const char *s1, const char *s2, u32 n)
{
    if (n == 0)
        return (0);
    do
    {
        if (*s1 != *s2++)
            return (*(const unsigned char *)s1
                    - *(const unsigned char *)(s2 - 1));
        if (*s1++ == 0)
            break;
    } while (--n != 0);
    return 0;
}