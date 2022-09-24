#include <levi/fs/file.h>
#include <levi/syscall/fdfunc.h>
#include <levi/syscall/syscall.h>

u64 sys_fdfunc(proc_t *proc, u64 args0, u64 args1, u64 args2, u64 args3)
{
    s64 result = fdfunc(args0, args1, args2, args3, proc);

    return result;
}
