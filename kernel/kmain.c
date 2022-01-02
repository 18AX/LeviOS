#include "levi/arch.h"
#include "levi/memory/memory.h"
#include "levi/memory/page_alloc.h"
#include "levi/memory/vmm.h"
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

    vas_t kernel_vas;

    if (fill_empty_vas(&kernel_vas) == FAILED)
    {
        term_print("Failed to initialize kernel virtual address space");
        die();
    }

    // Map all virtual memory
    memory_init(&kernel_vas);

    term_print("Kernel address space mapped\n");

    void *ptr = kmalloc(sizeof(u16) * 10);

    term_print("ptr %p\n", ptr);

    if (arch_init(boot_info) == FAILED)
    {
        die();
    }

    // Unreachable code.
    die();
}