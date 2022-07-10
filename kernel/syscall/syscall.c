#include <levi/arch/x86_64/syscall64.h>
#include <levi/memory/memory.h>
#include <levi/syscall/exec.h>
#include <levi/syscall/syscall.h>
#include <levi/utils/string.h>

static u8 syscall_stack[SYSCALL_STACK_SIZE];

static syscall_handler_t syscall_handlers[SYSCALL_NBR] = {
    [SYSCALL_EXEC] = sys_exec,
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

void *syscall_stack_addr(void)
{
    return (void *)(syscall_stack + SYSCALL_STACK_SIZE);
}
