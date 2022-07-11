#ifndef SYSCALL_HEADER
#define SYSCALL_HEADER

#include <levi/proc/process.h>
#include <levi/types.h>

#define SYSCALL_STACK_SIZE 1048576

#define SYSCALL_NBR 10

#define SYSCALL_OPEN 0
#define SYSCALL_READ 1
#define SYSCALL_WRITE 2
#define SYSCALL_CLOSE 3
#define SYSCALL_LSEEK 4
#define SYSCALL_DUP2 5
#define SYSCALL_EXEC 5
#define SYSCALL_EXIT 7
#define SYSCALL_HEAP_ALLOC 8
#define SYSCALL_HEAP_FREE 9

#define SYSCALL_FAILED 0xFFFFFFFFFFFFFFFF

typedef u64 (*syscall_handler_t)(proc_t *proc, u64 args0, u64 args1, u64 args2,
                                 u64 args3);

u64 syscall(proc_t *proc, u64 syscall_id, u64 args0, u64 args1, u64 args2,
            u64 args3);

STATUS syscall_init(void);

STATUS cpy_from_proc(proc_t *proc, void *dest, void *src, u64 size);

STATUS cpy_to_proc(proc_t *proc, void *dest, void *src, u64 size);

void *syscall_stack_addr(void);

#endif