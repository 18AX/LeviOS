#include <levi/fs/file.h>
#include <levi/syscall/flush.h>
#include <levi/syscall/syscall.h>

u64 sys_flush(proc_t *proc, u64 fd, u64 unused0, u64 unused1, u64 unused2)
{
    (void)unused0;
    (void)unused1;
    (void)unused2;

    s32 ret = flush(fd, proc);

    if (ret < 0)
    {
        return SYSCALL_FAILED;
    }

    return ret;
}