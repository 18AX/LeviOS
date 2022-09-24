#include <levi/arch/x86_64/apic.h>
#include <levi/arch/x86_64/io.h>
#include <levi/arch/x86_64/pic.h>
#include <levi/drivers/kbd/intel8042.h>
#include <levi/drivers/kbd/kbd.h>
#include <levi/fs/fs.h>
#include <levi/interrupts/interrupts.h>
#include <levi/memory/memory.h>
#include <levi/utils/queue.h>
#include <levi/utils/string.h>

static file_t *__open(vfs *vfs, const char *name, u32 flags);
static void __destroy_file(file_t *file);
static s64 __fdfunc(file_t *file, u64 func_id, u64 args0, u64 args1);

static struct vfs_operation kbd_operation = { .mkdir = NULL,
                                              .rmdir = NULL,
                                              .rename = NULL,
                                              .open = __open,
                                              .write = NULL,
                                              .destroy_file = __destroy_file,
                                              .flush = NULL,
                                              .read = NULL,
                                              .lseek = NULL,
                                              .fdfunc = __fdfunc };

static vfs kbd_vfs = { .data = NULL,
                       .flags = 0x0,
                       .name = "intelkbd",
                       .operation = &kbd_operation };

static queue_t *queue = NULL;

static enum keycode translate_to_keycode(u8 key)
{
    switch (key)
    {
    case 16:
        return KEYCODE_A;
    case 48:
        return KEYCODE_B;
    case 46:
        return KEYCODE_C;
    case 18:
        return KEYCODE_E;
    case 33:
        return KEYCODE_F;
    case 34:
        return KEYCODE_G;
    case 35:
        return KEYCODE_H;
    case 23:
        return KEYCODE_I;
    case 36:
        return KEYCODE_J;
    case 37:
        return KEYCODE_K;
    case 38:
        return KEYCODE_L;
    case 39:
        return KEYCODE_M;
    case 49:
        return KEYCODE_N;
    case 24:
        return KEYCODE_O;
    case 25:
        return KEYCODE_P;
    case 30:
        return KEYCODE_Q;
    case 19:
        return KEYCODE_R;
    case 31:
        return KEYCODE_S;
    case 20:
        return KEYCODE_T;
    case 22:
        return KEYCODE_U;
    case 47:
        return KEYCODE_V;
    case 44:
        return KEYCODE_W;
    case 45:
        return KEYCODE_X;
    case 21:
        return KEYCODE_Y;
    case 28:
        return KEYCODE_ENTER;
    case 58:
        return KEYCODE_SHIFT;
    case 75:
        return KEYCODE_ARROW_LEFT;
    case 77:
        return KEYCODE_ARROW_RIGHT;
    case 72:
        return KEYCODE_ARROW_UP;
    case 80:
        return KEYCODE_ARROW_DOWN;
    case 15:
        return KEYCODE_SHIFT;
    default:
        break;
    }
    return KEYCODE_UNKNOWN;
}

static void irq_handler(u64 id, u64 error_code, proc_t *proc)
{
    (void)id;
    (void)error_code;
    (void)proc;

    u8 status = inb(INTEL8042_STATUS);

    while ((status & 1) == 0)
        ;

    u8 data = inb(INTEL8042_READ);

    u8 key = data & 0x7F;

    if (((data)&0x80) == 0) // Key press
    {
        queue_add(queue, (u64)translate_to_keycode(key));
    }
}

STATUS intel8042_init()
{
    queue = queue_init(INTEL8042_QUEUE_SIZE);

    if (queue == NULL)
    {
        return FAILED;
    }

    if (register_interrupt_handler(INTEL8042_IRQ, irq_handler) == FAILED)
    {
        return FAILED;
    }

    if (ioapic_irq_set(0, 1, INTEL8042_IRQ,
                       IOAPIC_TRIGGER_HIGH | IOAPIC_TRIGGER_EDGE)
        == FAILED)
    {
        return FAILED;
    }

    return register_fs(&kbd_vfs);
}

static file_t *__open(vfs *vfs, const char *name, u32 flags)
{
    (void)vfs;
    (void)name;
    (void)flags;

    file_t *file = kmalloc(sizeof(file_t));

    if (file == NULL)
    {
        return NULL;
    }

    memset(file, 0, sizeof(file_t));

    return file;
}

static s64 __fdfunc(file_t *file, u64 func_id, u64 args0, u64 args1)
{
    (void)file;
    (void)args0;
    (void)args1;

    if (func_id != FDFUNC_GETKEY)
    {
        return -1;
    }

    u64 value = 0;

    if (queue_pop(queue, &value) == FAILED)
    {
        return -1;
    }

    return (enum keycode)value;
}

static void __destroy_file(file_t *file)
{
    kfree(file);
}