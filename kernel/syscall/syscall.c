#include <levi/arch/x86_64/syscall64.h>
#include <levi/memory/memory.h>
#include <levi/syscall/close.h>
#include <levi/syscall/dup2.h>
#include <levi/syscall/exec.h>
#include <levi/syscall/exit.h>
#include <levi/syscall/fdfunc.h>
#include <levi/syscall/flush.h>
#include <levi/syscall/getticks.h>
#include <levi/syscall/gettime.h>
#include <levi/syscall/mmap.h>
#include <levi/syscall/open.h>
#include <levi/syscall/read.h>
#include <levi/syscall/syscall.h>
#include <levi/syscall/write.h>
#include <levi/utils/string.h>

static u8 syscall_stack[SYSCALL_STACK_SIZE];

static syscall_handler_t syscall_handlers[SYSCALL_NBR] = {
    [SYSCALL_OPEN] = sys_open,         [SYSCALL_READ] = sys_read,
    [SYSCALL_WRITE] = sys_write,       [SYSCALL_FLUSH] = sys_flush,
    [SYSCALL_CLOSE] = sys_close,       [SYSCALL_DUP2] = sys_dup2,
    [SYSCALL_EXEC] = sys_exec,         [SYSCALL_EXIT] = sys_exit,
    [SYSCALL_MMAP] = sys_mmap,         [SYSCALL_GETTIME] = sys_gettime,
    [SYSCALL_GETTICKS] = sys_getticks, [SYSCALL_FDFUNC] = sys_fdfunc
};

u64 syscall(proc_t *proc, u64 syscall_id, u64 args0, u64 args1, u64 args2,
            u64 args3)
{
    if (syscall_id >= SYSCALL_NBR)
    {
        return SYSCALL_FAILED;
    }

    return syscall_handlers[syscall_id](proc, args0, args1, args2, args3);
}

STATUS cpy_from_proc(proc_t *proc, void *dest, void *src, u64 size)
{
    u64 phys_address = 0x0;

    if (vma_to_phys(&proc->vas, (u64)src, &phys_address) == MAP_FAILED)
    {
        return FAILED;
    }

    u64 kern_addr = phys_to_hhdm(phys_address);

    memcpy(dest, (void *)kern_addr, size);

    return SUCCESS;
}

STATUS cpy_to_proc(proc_t *proc, void *dest, void *src, u64 size)
{
    u64 phys_address = 0x0;

    if (vma_to_phys(&proc->vas, (u64)dest, &phys_address) == MAP_FAILED)
    {
        return FAILED;
    }

    u64 kern_addr = phys_to_hhdm(phys_address);

    memcpy((void *)kern_addr, src, size);

    return SUCCESS;
}

void *ptr_from_proc(proc_t *proc, void *ptr)
{
    u64 phys_address = 0x0;

    if (vma_to_phys(&proc->vas, (u64)ptr, &phys_address) == MAP_FAILED)
    {
        return NULL;
    }

    return (void *)phys_to_hhdm(phys_address);
}

void *syscall_stack_addr(void)
{
    return (void *)(syscall_stack + SYSCALL_STACK_SIZE);
}
