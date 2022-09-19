#ifndef TIMER_HEADER
#define TIMER_HEADER

#include <levi/proc/process.h>
#include <levi/types.h>

u64 sys_getticks(proc_t *proc, u64 unused0, u64 unused1, u64 unused2,
                 u64 unused3);

#endif
