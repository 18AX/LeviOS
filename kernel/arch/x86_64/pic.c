#include <levi/arch/x86_64/io.h>
#include <levi/arch/x86_64/pic.h>

void pic_disable()
{
    // 00010001 ICW4 present + cascade mode + edge trigered mode
    outb(PIC_MASTER_A, 0x11);
    outb(PIC_SLAVE_A, 0x11);

    outb(PIC_MASTER_B, PIC_MASTER_OFFSET);
    outb(PIC_SLAVE_B, PIC_SLAVE_OFFSET);

    outb(PIC_MASTER_B, 0x4);
    outb(PIC_SLAVE_B, 0x2);

    outb(PIC_MASTER_B, 0x1);
    outb(PIC_SLAVE_B, 0x1);

    // Mask all irq
    outb(PIC_MASTER_B, 0xFF);
    outb(PIC_SLAVE_B, 0xFF);
}