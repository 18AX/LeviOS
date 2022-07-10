#ifndef EXEC_HEADER
#define EXEC_HEADER

#include <levi/proc/process.h>
#include <levi/types.h>

#define USER_STACK_ADDRESS 0x7ff000000000
#define USER_STACK_PAGE_NB 2048 /** 8388608 bytes **/

/**
 * @brief exec syscall
 *
 * @param proc
 * @param args0 path to binary
 * @param args1 process name
 * @param args2 argv
 * @param args3 envp
 * @return u64 return process id
 */
u64 sys_exec(proc_t *proc, u64 args0, u64 args1, u64 args2, u64 args3);

u64 kexec(const char *path, const char *proc_name, const char *argv[],
          const char *envp[]);

#endif