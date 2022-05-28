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

u64 syscall64_exec(u64 id, u64 args0, u64 args1, u64 args2, u64 args3)
{
    register u64 syscall_id asm("rax") = id;
    register u64 rdi asm("rdi") = args0;
    register u64 rsi asm("rsi") = args1;
    register u64 rdx asm("rdx") = args2;
    register u64 r10 asm("r10") = args3;

    (void)syscall_id;
    (void)rdi;
    (void)rsi;
    (void)rdx;
    (void)r10;

    asm volatile("int $0x80");

    return syscall_id;
}