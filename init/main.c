#include <stdio.h>
#include <string.h>
#include <syscall.h>

int main(void)
{
    printf("^rHello ^bFrom ^mUserland!\n");

    struct time time = { 0 };

    gettime(&time);

    printf("current time: %u:%u:%u %u/%u/%u\n", time.hours, time.minutes,
           time.seconds, time.day, time.month, time.year);

    for (;;)
    {
        asm volatile("pause");
    }

    return 0;
}