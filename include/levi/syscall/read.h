#ifndef READ_HEADER
#define READ_HEADER

#include <levi/proc/process.h>
#include <levi/types.h>

u64 sys_read(proc_t *proc, u64 fd, u64 buffer, u64 size, u64 unused);

#endif