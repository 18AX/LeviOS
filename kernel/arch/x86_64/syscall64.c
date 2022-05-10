#include <levi/arch/x86_64/syscall64.h>
#include <levi/interrupts/interrupts.h>
#include <levi/syscall/syscall.h>

static void syscall_handler(u64 id, u64 error_code, regs_t *regs)
{
    (void)id;
    (void)error_code;
    u64 result = syscall(regs->rax, regs->rdi, regs->rsi, regs->rdx, regs->r10);

    regs->rax = result;
}

void syscall64_init()
{
    register_interrupt_handler(SYSCALL_GATE, syscall_handler);
}