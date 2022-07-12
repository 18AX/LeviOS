#include <levi/fs/file.h>
#include <levi/syscall/open.h>
#include <levi/syscall/syscall.h>

u64 sys_open(proc_t *proc, u64 path, u64 len, u64 flags, u64 unused)
{
    (void)unused;

    char buffer[PATH_MAX_LEN];

    if (len + 1 >= PATH_MAX_LEN)
    {
        return SYSCALL_FAILED;
    }

    /** copy the string and the '\0' **/
    cpy_from_proc(proc, buffer, (void *)path, len + 1);

    s32 fd = open(buffer, flags, proc);

    if (fd < 0)
    {
        return SYSCALL_FAILED;
    }

    return fd;
}