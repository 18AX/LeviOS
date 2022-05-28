#include <levi/arch.h>
#include <levi/arch/x86_64/acpi.h>
#include <levi/arch/x86_64/gdt.h>
#include <levi/arch/x86_64/idt.h>
#include <levi/arch/x86_64/pci.h>
#include <levi/arch/x86_64/pic.h>
#include <levi/arch/x86_64/syscall64.h>
#include <levi/utils/string.h>

STATUS arch_init(struct stivale2_struct *boot_info)
{
    gdt_init();
    idt_init();

    if (acpi_init(boot_info) == FAILED)
    {
        return FAILED;
    }
    // This will be probably be replaced with apic.
    pic_init();

#if 0
    if (pci_init() == FAILED)
    {
        return FAILED;
    }
#endif

    syscall64_init();

    return SUCCESS;
}

void arch_init_ctx(context_t *ctx, void *entry, void *stack, u8 is_kernel)
{
    memset(ctx, 0x0, sizeof(context_t));

    ctx->cs = is_kernel ? SEGMENT_SELECTOR(1, 0, 0) : SEGMENT_SELECTOR(3, 0, 3);
    ctx->ss = is_kernel ? SEGMENT_SELECTOR(2, 0, 0) : SEGMENT_SELECTOR(4, 0, 3);
    ctx->rip = (u64)entry;
    ctx->rsp = (u64)stack;
    ctx->regs.rbp = (u64)stack;
    ;
}