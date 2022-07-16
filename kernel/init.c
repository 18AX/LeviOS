#include <levi/init.h>
#include <levi/syscall/exec.h>

STATUS run_init(struct stivale2_struct *boot_info)
{
    (void)boot_info;

    kexec("memfs:init", "init", NULL, NULL);

    return SUCCESS;
}