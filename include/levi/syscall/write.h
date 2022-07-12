#ifndef WRITE_HEADER
#define WRITE_HEADER

#include <levi/proc/process.h>
#include <levi/types.h>

u64 sys_write(proc_t *proc, u64 fd, u64 buffer, u64 size, u64 unused);

#endif