#ifndef EXIT_HEADER
#define EXIT_HEADER

#include <levi/proc/process.h>
#include <levi/types.h>

u64 sys_exit(proc_t *proc,
             u64 status_code,
             u64 unused0,
             u64 unused1,
             u64 unused2);

#endif