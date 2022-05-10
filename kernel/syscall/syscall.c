#include <levi/syscall/syscall.h>

static syscall_handler_t syscall_handlers[SYSCALL_NBR] = { 0x0 };

u64 syscall(u64 syscall_id, u64 args0, u64 args1, u64 args2, u64 args3)
{
    if (syscall_id >= SYSCALL_NBR)
    {
        return SYSCALL_FAILED;
    }

    return syscall_handlers[syscall_id](args0, args1, args2, args3);
}