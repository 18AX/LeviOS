#include <levi/syscall/getticks.h>
#include <levi/time/time.h>

u64 sys_getticks(proc_t *proc, u64 unused0, u64 unused1, u64 unused2,
                 u64 unused3)
{
    (void)proc;
    (void)unused0;
    (void)unused1;
    (void)unused2;
    (void)unused3;

    return timer_count();
}