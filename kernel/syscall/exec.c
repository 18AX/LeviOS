#include <levi/syscall/exec.h>

STATUS exec_from_fd(s32 fd, u32 flags)
{
    (void)fd;
    (void)flags;
    return SUCCESS;
}