#ifndef SYSCALL_HEADER
#define SYSCALL_HEADER

#include <levi/proc/process.h>
#include <levi/types.h>

#define SYSCALL_NBR 4

#define SYSCALL_OPEN 0
#define SYSCALL_READ 1
#define SYSCALL_WRITE 2
#define SYSCALL_EXEC 3

#define SYSCALL_FAILED -1

typedef u64 (*syscall_handler_t)(proc_t *proc, u64 args0, u64 args1, u64 args2,
                                 u64 args3);

u64 syscall(proc_t *proc, u64 syscall_id, u64 args0, u64 args1, u64 args2,
            u64 args3);

STATUS syscall_init(void);

STATUS cpy_from_proc(proc_t *proc, void *dest, void *src, u64 size);

STATUS cpy_to_proc(proc_t *proc, void *dest, void *src, u64 size);

u64 ksyscall(u64 syscall_id, u64 args0, u64 args1, u64 args2, u64 args3);

#endif