#include <elf.h>
#include <levi/fs/file.h>
#include <levi/init.h>
#include <levi/memory/memory.h>
#include <levi/proc/scheduler.h>
#include <levi/syscall/exec.h>
#include <levi/types.h>
#include <levi/utils/kerr.h>
#include <levi/utils/kprintf.h>
#include <link.h>

STATUS run_init(struct stivale2_struct *boot_info)
{
    (void)boot_info;

    kprintf("Running exec...\n");

    u64 id = kexec("memfs:init", "init", NULL, NULL);

    kprintf("[^ginfo^w] Init finished %ld\n", id);

    kprintf("Returned from init\n");

    return SUCCESS;
}