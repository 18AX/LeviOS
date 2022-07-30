#include <levi/fs/file.h>
#include <levi/syscall/close.h>
#include <levi/syscall/syscall.h>

u64 sys_close(proc_t *proc, u64 fd, u64 unused0, u64 unused1, u64 unused2)
{
    (void)unused0;
    (void)unused1;
    (void)unused2;

    u64 res = close(fd, proc);

    if (res == SUCCESS)
    {
        return 0;
    }

    return SYSCALL_FAILED;
}