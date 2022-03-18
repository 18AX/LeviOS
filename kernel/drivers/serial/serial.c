#include "levi/drivers/serial/serial.h"

#include "levi/arch/x86_64/io.h"
#include "levi/fs/fs.h"
#include "levi/memory/memory.h"
#include "levi/stivale2.h"
#include "levi/utils/string.h"

static file_t *__open(const char *name, u32 flags);
static s32 __write(file_t *file, u8 *buffer, u32 size);
static void __destroy_file(file_t *file);

struct vfs_operation serial_operation = { .mkdir = NULL,
                                          .rmdir = NULL,
                                          .rename = NULL,

                                          .open = __open,
                                          .write = __write,
                                          .destroy_file = __destroy_file,
                                          .flush = NULL,
                                          .read = NULL,
                                          .lseek = NULL };

static vfs serial_vfs = { .name = "serial",
                          .flags = 0x0,
                          .operation = &serial_operation };

STATUS serial_init()
{
    term_print("SERIAL INIT\n");
    return register_fs(&serial_vfs);
}

static file_t *__open(const char *name, u32 flags)
{
    if (strcmp(name, "COM1"))
    {
        outb(COM1 + 3, 0x80); // Set DLAB to 1
        outb(COM1 + 0, 0x00); // Divisor low byte for 38400 bps
        outb(COM1 + 1, 0x03); // Divisor high byte for 38400 bps
        outb(COM1 + 3, 0x03);

        outb(COM1, 0x03); // 00000011: No parity and 8 bits
        outb(COM1 + 2,
             0xC7); // 11000111: interrupt trigger level 14 bytes, Clear
                    // transmit fifo (bit 2), Clear Receive fifo (bit 1), enable
                    // fifo (bit 0)

        file_t *file = kmalloc(sizeof(file_t));
        file->data = (void *)COM1;
        file->vfs = &serial_vfs;
        file->flags = flags;
        file->cursor_position = 0;
        file->open_count = 0;

        return file;
    }

    return NULL;
}

static s32 __write(file_t *file, u8 *buffer, u32 size)
{
    u64 w = 0;

    u16 port = (u16)((u64)file->data);

    for (; w < size; ++w)
    {
        // Waiting for the serial to be availabe for writing
        while ((inb(port + 5) & (1 << 5)) == 0)
            ;

        // Write the data to the serial port
        outb(port, buffer[w]);
    }

    return w;
}

static void __destroy_file(file_t *file)
{
    kfree(file);
}