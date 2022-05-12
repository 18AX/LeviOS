#include <levi/arch/x86_64/syscall64.h>
#include <levi/interrupts/interrupts.h>
#include <levi/syscall/syscall.h>

static void syscall_handler(u64 id, u64 error_code, proc_t *proc)
{
    (void)id;
    (void)error_code;
    u64 result =
        syscall(proc, proc->ctx.regs.rax, proc->ctx.regs.rdi,
                proc->ctx.regs.rsi, proc->ctx.regs.rdx, proc->ctx.regs.r10);

    proc->ctx.regs.rax = result;
}

void syscall64_init()
{
    register_interrupt_handler(SYSCALL_GATE, syscall_handler);
}