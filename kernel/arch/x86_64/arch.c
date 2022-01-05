#include "levi/arch.h"

#include "levi/arch/x86_64/idt.h"

STATUS arch_init(struct stivale2_struct *boot_info)
{
    (void)boot_info;

    idt_init();

    return SUCCESS;
}