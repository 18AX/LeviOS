#include "levi/devices/kbd/intel8042.h"

#include "levi/arch/x86_64/io.h"
#include "levi/arch/x86_64/pic.h"
#include "levi/devices/kbd.h"
#include "levi/interrupts/interrupts.h"

static enum keycode translate_to_keycode(u8 key)
{
    switch (key)
    {
    case 30:
        return KEYCODE_A;
    case 32:
        return KEYCODE_D;
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
        on_key_press(translate_to_keycode(key));
    }
    else // Key released
    {
        on_key_release(translate_to_keycode(key));
    }

    // pic_eoi(PIC_MASTER_A);
}

void intel8042_init()
{
    register_interrupt_handler(PIC_MASTER_OFFSET + 1, irq_handler);
    // pic_set_irq(PIC_MASTER_B, 0x1, IRQ_UNMASK);
}