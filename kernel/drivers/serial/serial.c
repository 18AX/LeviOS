#include <levi/arch/x86_64/io.h>
#include <levi/drivers/serial/serial.h>
#include <levi/fs/fs.h>
#include <levi/memory/memory.h>
#include <levi/serial/serial.h>
#include <levi/utils/string.h>

static file_t *__open(vfs *vfs, const char *name, u32 flags);
static s64 __write(file_t *file, u8 *buffer, u64 size);
static void __destroy_file(file_t *file);

static struct vfs_operation serial_operation = { .mkdir = NULL,
                                                 .rmdir = NULL,
                                                 .rename = NULL,

                                                 .open = __open,
                                                 .write = __write,
                                                 .destroy_file = __destroy_file,
                                                 .flush = NULL,
                                                 .read = NULL,
                                                 .lseek = NULL,
                                                 .fdfunc = NULL };

static vfs serial_vfs = { .name = "serial",
                          .flags = 0x0,
                          .data = NULL,
                          .operation = &serial_operation };

STATUS serial_init()
{
    return register_fs(&serial_vfs);
}

static file_t *__open(vfs *vfs, const char *name, u32 flags)
{
    (void)vfs;

    if ((flags & FS_WRITE) == 0)
    {
        return NULL;
    }

    if (strcmp(name, "COM1") == 0)
    {
        serial_init_port(COM1);

        file_t *file = kmalloc(sizeof(file_t));

        if (file == NULL)
        {
            return NULL;
        }

        file->data = (void *)COM1;

        return file;
    }

    return NULL;
}

static s64 __write(file_t *file, u8 *buffer, u64 size)
{
    if (file == NULL)
    {
        return -1;
    }

    return serial_write((u16)(u64)file->data, buffer, size);
}

static void __destroy_file(file_t *file)
{
    kfree(file);
}