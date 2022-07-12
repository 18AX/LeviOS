#ifndef OPEN_HEADER
#define OPEN_HEADER

#include <levi/proc/process.h>
#include <levi/types.h>

/**
 * @brief Open a file system
 *
 * @param proc
 * @param args0 filesystem path
 * @param args1 filesystem path len
 * @param args2 flags
 * @param args3 not used
 * @return u64
 */
u64 sys_open(proc_t *proc, u64 args0, u64 args1, u64 args2, u64 args3);

#endif