#include <levi/fs/file.h>
#include <levi/syscall/read.h>
#include <levi/syscall/syscall.h>

u64 sys_read(proc_t *proc, u64 fd, u64 buffer, u64 size, u64 unused)
{
    (void)unused;

    void *ptr = ptr_from_proc(proc, (void *)buffer);

    if (ptr == NULL)
    {
        return SYSCALL_FAILED;
    }

    s64 ret = read(fd, ptr, size, proc);

    if (ret < 0)
    {
        return SYSCALL_FAILED;
    }

    return ret;
}