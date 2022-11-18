#include <levi/proc/scheduler.h>
#include <levi/syscall/exit.h>
#include <levi/syscall/syscall.h>

u64 sys_exit(proc_t *proc,
             u64 status_code,
             u64 unused0,
             u64 unused1,
             u64 unused2)
{
    (void)status_code; /** For now we do nothing with the status code **/
    (void)unused0;
    (void)unused1;
    (void)unused2;

    sched_remove(proc->id);
    proc_destroy(proc);

    return 0;
}