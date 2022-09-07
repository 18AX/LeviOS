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
#include <levi/utils/kerr.h>
#include <levi/utils/kprintf.h>

static STATUS early_init(struct stivale2_struct *boot_info)
{
    interrupts_disable();

    interrupt_init();

    if (arch_init(boot_info) == FAILED)
    {
        return FAILED;
    }

    if (kframe_init(boot_info) == FAILED)
    {
        return FAILED;
    }

    vas_t kernel_vas;

    if (fill_empty_vas(&kernel_vas) == FAILED)
    {
        return FAILED;
    }

    // Map all virtual memory
    memory_init(&kernel_vas);

    proc_t *kernel_proc = proc_kernel("Kernel", &kernel_vas);

    if (kernel_proc == NULL)
    {
        return FAILED;
    }

    sched_init();
    sched_add(kernel_proc->id);

    return SUCCESS;
}

static STATUS init_kernel_io()
{
    int fd = kopen("console:", FS_READ | FS_WRITE);

    if (fd < 0)
    {
        return FAILED;
    }

    if (kdup2(fd, STDOUT_FILENO) < 0)
    {
        return FAILED;
    }

    if (kdup2(fd, STDERR_FILENO) < 0)
    {
        return FAILED;
    }

    kclose(fd);

    return SUCCESS;
}

static STATUS init(struct stivale2_struct *boot_info)
{
    if (init_fs() == FAILED)
    {
        return FAILED;
    }

    if (memfs_init() == FAILED)
    {
        return FAILED;
    }

    if (drivers_init(boot_info) == FAILED)
    {
        return FAILED;
    }

    kbd_init();

    return SUCCESS;
}

void main(struct stivale2_struct *boot_info)
{
    if (early_init(boot_info) == FAILED)
    {
        die();
    }

    if (init(boot_info) == FAILED)
    {
        die();
    }

    if (init_kernel_io() == FAILED)
    {
        die();
    }

    kprintf("[^cinfo^w] Kernel initialized\n");

    u32 module_loaded = module_init(boot_info);

    kprintf("[^cinfo^w] %u modules loaded\n", module_loaded);

    kprintf("[^g ok ^w] Welcome to LeviOS\n");

    kprintf("[^cinfo^w] Start memfs:init\n");

    if (run_init(boot_info) == FAILED)
    {
        kerr(KERROR_UNKNOW, "Failed to run init\n");
    }

    interrupts_enable();

    sched_start();

    for (;;)
        ;

    asm volatile("xchgw %bx, %bx");

    // Unreachable code.
    die();
}