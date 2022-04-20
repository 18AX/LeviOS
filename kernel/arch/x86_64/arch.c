#include "levi/arch.h"

#include <levi/arch/x86_64/acpi.h>

#include "levi/arch/x86_64/idt.h"
#include "levi/arch/x86_64/pic.h"

STATUS arch_init(struct stivale2_struct *boot_info)
{
    idt_init();

    if (acpi_init(boot_info) == FAILED)
    {
        return FAILED;
    }
    // This will be probably be replaced with apic.
    pic_init();

    return SUCCESS;
}