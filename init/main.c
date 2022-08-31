#include <stdio.h>
#include <string.h>
#include <syscall.h>

int main(void)
{
    printf("^rHello ^bFrom ^mUserland!\n");

    for (;;)
    {
        asm volatile("pause");
    }

    return 0;
}