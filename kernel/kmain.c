#include <levi/arch.h>
#include <levi/devices/kbd.h>
#include <levi/drivers/drivers.h>
#include <levi/fs/file.h>
#include <levi/fs/fs.h>
#include <levi/interrupts/interrupts.h>
#include <levi/memory/memory.h>
#include <levi/memory/page_alloc.h>
#include <levi/memory/vmm.h>
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
    proc_t *kernel_proc =
        process_create("Levi", NULL, &kernel_vas, PROCESS_KERNEL);

    if (kernel_proc == NULL)
    {
        term_print("Failed to create kernel process\n");
    }

    sched_set(kernel_proc->id);

    return SUCCESS;
}

void main(struct stivale2_struct *boot_info)
{
    if (early_init(boot_info) == FAILED)
    {
        term_print("Failed to initialize kernel\n");
        die();
    }

    interrupts_enable();

    term_print("After interrupt\n");

    if (init_fs() == FAILED)
    {
        term_print("Failed to initialize file system\n");
    }

    if (drivers_init() == FAILED)
    {
        term_print("Failed to initialize drivers\n");
    }

    s32 fd = kopen("serial:COM1", O_WRONLY);

    if (fd == -1)
    {
        term_print("Failed to open serial\n");
    }

    char data[] = "TOTO";

    kwrite(fd, data, 4);

    kbd_init();

    u64 i = 0;
#if 0
    asm volatile (
        "mov $4,%%rdi\n"
        "int $80\n" : : : "rdi"
    );

#endif

    for (;;)
    {
#if 1
        if (i % 10000000 == 0)
        {
            term_print("HERE %ld %ld\n", get_state(), get_state() & KEYCODE_A);
        }
#endif
        if (key_state(KEYCODE_A) == KEY_PRESS)
        {
            kwrite(fd, data, 4);
        }

        i++;
    }

    kclose(fd);

    term_print("WTF\n");

    // Unreachable code.
    die();
}