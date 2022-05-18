#include <levi/arch.h>
#include <levi/devices/kbd.h>
#include <levi/drivers/drivers.h>
#include <levi/fs/file.h>
#include <levi/fs/fs.h>
#include <levi/fs/memfs.h>
#include <levi/init.h>
#include <levi/interrupts/interrupts.h>
#include <levi/memory/memory.h>
#include <levi/memory/page_alloc.h>
#include <levi/memory/vmm.h>
#include <levi/modules/module.h>
#include <levi/panic.h>
#include <levi/proc/process.h>
#include <levi/proc/scheduler.h>
#include <levi/stivale2.h>
#include <levi/types.h>

static STATUS early_init(struct stivale2_struct *boot_info)
{
    if (term_init(boot_info) == FAILED)
    {
        return FAILED;
    }

    term_print("Initializing LeviOS...\n");

    term_print("stivale2 boot structure 0x%lx\n", boot_info);

    if (kframe_init(boot_info) == FAILED)
    {
        term_print("Failed to init frame allocator");
        return FAILED;
    }

    kframe_dump();

    vas_t kernel_vas;

    if (fill_empty_vas(&kernel_vas) == FAILED)
    {
        term_print("Failed to initialize kernel virtual address space");
        return FAILED;
    }

    // Map all virtual memory
    memory_init(&kernel_vas);

    term_print("Kernel address space mapped\n");

    interrupt_init();

    if (arch_init(boot_info) == FAILED)
    {
        term_print("Failed to initialize arch\n");
        return FAILED;
    }

    term_print("Arch intialized\n");

    proc_t *kernel_proc = proc_kernel("Kernel", &kernel_vas);

    if (kernel_proc == NULL)
    {
        term_print("Failed to create kernel process\n");
    }

    sched_set(kernel_proc->id);

    return SUCCESS;
}

static STATUS init(struct stivale2_struct *boot_info)
{
    term_print("After interrupt\n");

    if (init_fs() == FAILED)
    {
        term_print("Failed to initialize file system\n");
    }

    if (memfs_init() == FAILED)
    {
        term_print("Failed to initialize memfs\n");
        return FAILED;
    }

    term_print("Memfs initialized\n");

    if (drivers_init() == FAILED)
    {
        term_print("Failed to initialize drivers\n");
    }

    term_print("Drivers initialized\n");

    kbd_init();

    u32 nbr_modules = module_init(boot_info);

    term_print("%u modules loaded\n", nbr_modules);

    return SUCCESS;
}

void main(struct stivale2_struct *boot_info)
{
    if (early_init(boot_info) == FAILED)
    {
        term_print("Failed to initialize kernel\n");
        die();
    }

    if (init(boot_info) == FAILED)
    {
        term_print("Failed to initialize kernel\n");
        die();
    }

    interrupts_enable();

    for (;;)
    {
        u64 key = get_kbd_state();
        term_print("\rkeystate %lx", key);
        asm volatile("hlt");
    }

    term_print("Run init\n");

    if (run_init(boot_info) == FAILED)
    {
        term_print("Failed to run init program\n");
    }

    // Unreachable code.
    die();
}