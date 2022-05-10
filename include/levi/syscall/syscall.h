#ifndef SYSCALL_HEADER
#define SYSCALL_HEADER

#include <levi/types.h>

#define SYSCALL_NBR 3

#define SYSCALL_OPEN 0
#define SYSCALL_READ 1
#define SYSCALL_WRITE 2

#define SYSCALL_FAILED -1

typedef u64 (*syscall_handler_t)(u64 args0, u64 args1, u64 args2, u64 args3);

u64 syscall(u64 syscall_id, u64 args0, u64 args1, u64 args2, u64 args3);

STATUS syscall_init(void);

#endif