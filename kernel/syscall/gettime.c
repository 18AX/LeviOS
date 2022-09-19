#include <levi/arch.h>
#include <levi/syscall/gettime.h>
#include <levi/syscall/syscall.h>

u64 sys_gettime(proc_t *proc, u64 address, u64 unused1, u64 unused2,
                u64 unused3)
{
    (void)unused1;
    (void)unused2;
    (void)unused3;

    struct time time = { 0 };

    arch_get_time(&time);

    cpy_to_proc(proc, (void *)address, &time, sizeof(struct time));

    return 0;
}