#ifndef FLUSH_HEADER
#define FLUSH_HEADER

#include <levi/proc/process.h>
#include <levi/types.h>

u64 sys_flush(proc_t *proc, u64 fd, u64 unused0, u64 unused1, u64 unused2);

#endif