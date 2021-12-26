#include "levi/arch.h"
#include "levi/panic.h"
#include "levi/stivale2.h"
#include "levi/types.h"

void main(struct stivale2_struct *boot_info)
{
    if (arch_init(boot_info) == FAILED)
    {
        die();
    }

    // Unreachable code.
    die();
}