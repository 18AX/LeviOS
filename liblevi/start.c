#include <stdio.h>
#include <syscall.h>
#include <types.h>

extern int main(void);

void __start_main(void)
{
    s32 fd = open("console:", FS_WRITE);

    if (dup2(fd, STDOUT_FD) < 0)
    {
        exit(0);
    }

    u64 ret = main();
    exit(ret);
}