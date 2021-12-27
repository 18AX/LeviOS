#include "levi/arch.h"
#include "levi/memory/page_alloc.h"
#include "levi/panic.h"
#include "levi/stivale2.h"
#include "levi/types.h"

void main(struct stivale2_struct *boot_info)
{
    if (term_init(boot_info) == FAILED)
    {
        die();
    }

    term_print("LeviOS kernel reached\n");
    term_print("stivale2_struct at 0x%p\n", boot_info);

    if (kframe_init(boot_info) == FAILED)
    {
        term_print("\n\nkframe_init failed");
        die();
    }

    kframe_dump();

    if (arch_init(boot_info) == FAILED)
    {
        die();
    }

    // Unreachable code.
    die();
}