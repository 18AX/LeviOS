#ifndef GET_TIME_HEADER
#define GET_TIME_HEADER

#include <levi/proc/process.h>
#include <levi/types.h>

u64 sys_gettime(proc_t *proc,
                u64 address,
                u64 unused1,
                u64 unused2,
                u64 unused3);

#endif