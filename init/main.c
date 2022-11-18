#include <stdio.h>
#include <string.h>
#include <syscall.h>

int main(void)
{
    printf("^rHello ^bFrom ^mUserland!\n");

    struct time time = { 0 };

    gettime(&time);

    printf("current time: %u:%u:%u %u/%u/%u\n",
           time.hours,
           time.minutes,
           time.seconds,
           time.day,
           time.month,
           time.year);

    s32 fd = open("intelkbd:toto", FS_READ);

    if (fd < 0)
    {
        printf("Failed to open the keyboard\n");
        return 1;
    }

    printf("Open keyboard %d\n", fd);

    for (;;)
    {
        u64 value = fdfunc(fd, 0x7ADF449E83A56016, 0, 0);
        if (value != (u64)-1)
        {
            printf("KEY %lu\n", value);
        }
    }

    for (;;)
    {
        asm volatile("pause");
    }

    return 0;
}