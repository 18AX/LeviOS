#include <string.h>

void *memcpy(void *dest, const void *src, u64 n)
{
    const char *s = src;
    char *d = dest;

    for (u64 i = 0; i < n; i++)
        *d++ = *s++;

    return dest;
}

void *memset(void *dest, int val, u64 len)
{
    u8 *ptr = dest;
    while (len-- > 0)
        *ptr++ = val;
    return dest;
}

void *memmove(void *dest, const void *src, u64 n)
{
    u8 *from = (u8 *)src;
    u8 *to = (u8 *)dest;

    if (from == to || n == 0)
        return dest;
    if (to > from && to - from < (s32)n)
    {
        /* to overlaps with from */
        /*  <from......>         */
        /*         <to........>  */
        /* copy in reverse, to avoid overwriting from */
        s32 i;
        for (i = n - 1; i >= 0; i--)
            to[i] = from[i];
        return dest;
    }
    if (from > to && from - to < (s32)n)
    {
        /* to overlaps with from */
        /*        <from......>   */
        /*  <to........>         */
        /* copy forwards, to avoid overwriting from */
        u64 i;
        for (i = 0; i < n; i++)
            to[i] = from[i];
        return dest;
    }
    memcpy(dest, src, n);
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

char *strncpy(char *destination, const char *source, u64 num)
{
    // return if no memory is allocated to the destination
    if (destination == NULL)
    {
        return NULL;
    }

    // take a pointer pointing to the beginning of the destination string
    char *ptr = destination;

    // copy first `num` characters of C-string pointed by source
    // into the array pointed by destination
    while (*source && num--)
    {
        *destination = *source;
        destination++;
        source++;
    }

    // null terminate destination string
    *destination = '\0';

    // the destination is returned by standard `strncpy()`
    return ptr;
}

char *strcpy(char *destination, const char *source)
{
    u32 i = 0;

    for (; source[i] != '\0'; ++i)
    {
        destination[i] = source[i];
    }

    destination[i] = '\0';

    return destination;
}

s32 strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const u8 *)s1 - *(const u8 *)s2;
}

char *strchr(char *str, char c)
{
    for (u32 i = 0; str[i] != '\0'; ++i)
    {
        if (str[i] == c)
        {
            return str + i;
        }
    }

    return NULL;
}