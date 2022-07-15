#include <levi/fs/file.h>
#include <levi/syscall/dup2.h>

u64 sys_dup2(proc_t *proc, u64 oldfd, u64 newfd, u64 unused0, u64 unused1)
{
    (void)unused0;
    (void)unused1;
    return dup2(oldfd, newfd, proc);
}