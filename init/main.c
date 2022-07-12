#include <string.h>
#include <syscall.h>

int main(void)
{
    s32 fd = open("console:", FS_WRITE);

    char str[] = "^rHello ^bFrom ^mUserland!";

    write(fd, str, strlen(str));

    flush(fd);

    if (fd < 0)
    {
        asm volatile("xchgw %bx, %bx");
    }

    for (;;)
    {
        asm volatile("pause");
    }

    return 0;
}