#include <levi/arch.h>
#include <levi/arch/x86_64/acpi.h>
#include <levi/arch/x86_64/apic.h>
#include <levi/arch/x86_64/cmos.h>
#include <levi/arch/x86_64/gdt.h>
#include <levi/arch/x86_64/hpet.h>
#include <levi/arch/x86_64/idt.h>
#include <levi/arch/x86_64/pci.h>
#include <levi/arch/x86_64/pic.h>
#include <levi/arch/x86_64/syscall64.h>
#include <levi/arch/x86_64/tss.h>
#include <levi/syscall/syscall.h>
#include <levi/utils/string.h>

STATUS arch_init(struct stivale2_struct *boot_info)
{
    gdt_init();
    idt_init();

    if (acpi_init(boot_info) == FAILED)
    {
        return FAILED;
    }

    if (hpet_init() == FAILED)
    {
        return FAILED;
    }

    // disable_pic();
    //  This will be probably be replaced with apic.
    pic_disable();

    if (apic_init() == FAILED)
    {
        return FAILED;
    }

    apic_enable();

    if (lapic_timer_init() == FAILED)
    {
        return FAILED;
    }

#if 0
    if (pci_init() == FAILED)
    {
        return FAILED;
    }
#endif

    syscall64_init();

    return SUCCESS;
}

void arch_ctx_init(context_t *ctx, void *entry, void *stack, u8 is_kernel)
{
    memset(ctx, 0x0, sizeof(context_t));

    ctx->isr_ctx.rip = (u64)entry;
    ctx->isr_ctx.rsp = (u64)stack;
    ctx->isr_ctx.regs.rbp = (u64)stack;

    if (is_kernel == 1)
    {
        ctx->isr_ctx.cs = SEGMENT_SELECTOR(1, 0, 0);
        ctx->isr_ctx.ss = SEGMENT_SELECTOR(2, 0, 0);
    }
    else
    {
        ctx->isr_ctx.rflags = RFLAGS_IF | RFLAGS_RESERVED1;
        ctx->isr_ctx.cs = SEGMENT_SELECTOR(3, 0, 3);
        ctx->isr_ctx.ss = SEGMENT_SELECTOR(4, 0, 3);
    }
}

extern void __ctx_set(struct isr_context *isr_ctx);

void arch_ctx_set(context_t *ctx)
{
    __ctx_set(&ctx->isr_ctx);
}

void arch_get_time(struct time *time)
{
    cmos_get_time(time);
}
