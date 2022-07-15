#include <stdio.h>
#include <string.h>
#include <syscall.h>

int main(void)
{
    printf("^rHello ^bFrom ^mUserland!");

    for (;;)
    {
        asm volatile("pause");
    }

    return 0;
}