#include <levi/fs/file.h>
#include <levi/init.h>
#include <levi/types.h>
#include <levi/utils/kerr.h>
#include <levi/utils/kprintf.h>

STATUS run_init(struct stivale2_struct *boot_info)
{
    (void)boot_info;

    s32 fd = kopen("memfs:init", FS_READ);

    if (fd < 0)
    {
        kerr(KERROR_NOT_FOUND, "module init not found");
    }

    kclose(fd);

    return SUCCESS;
}