#ifndef DUP2_HEADER
#define DUP2_HEADER

#include <levi/proc/process.h>
#include <levi/types.h>

u64 sys_dup2(proc_t *proc, u64 oldfd, u64 newfd, u64 unused0, u64 unused1);

#endif