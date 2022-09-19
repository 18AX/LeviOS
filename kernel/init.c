#include <levi/drivers/fs/iso9660.h>
#include <levi/fs/file.h>
#include <levi/init.h>
#include <levi/panic.h>
#include <levi/syscall/exec.h>
#include <levi/utils/kprintf.h>

STATUS run_init(struct stivale2_struct *boot_info)
{
    (void)boot_info;

    s32 fd = kopen("memfs:initfs", FS_READ);

    if (fd < 0)
    {
        kprintf("Failed to open memfs\n");
        return FAILED;
    }

    if (iso_mount(fd, "initfs") == FAILED)
    {
        kprintf("Failed to mount iso\n");
        kclose(fd);
        return FAILED;
    }

    kclose(fd);

    return kexec("initfs:init.elf", "init", NULL, NULL);
}