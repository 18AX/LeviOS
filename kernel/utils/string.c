#include "levi/utils/string.h"

#include "levi/memory/memory.h"

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

STATUS split(const char *str, char delimiter, char *output[])
{
    u32 i = 0;

    u32 len = strlen(str);

    for (; str[i] != '\0' && str[i] != delimiter; ++i)
        ;

    if (i == len)
    {
        return FAILED;
    }

    char *part1 = kmalloc(sizeof(char) * (i + 1));

    strncpy(part1, str, i);

    part1[i] = '\0';

    char *part2 = kmalloc(sizeof(char) * (len - i));

    strncpy(part2, str + i + 1, len - i - 1);

    part2[len - i - 1] = '\0';

    output[0] = part1;
    output[1] = part2;

    return SUCCESS;
}