#ifndef PIC_HEADER
#define PIC_HEADER

#define PIC_MASTER_A 0x20
#define PIC_MASTER_B 0x21
#define PIC_SLAVE_A 0xA0
#define PIC_SLAVE_B 0xA1

#define PIC_MASTER_OFFSET 0x60
#define PIC_SLAVE_OFFSET 0x68

#define PIC_EOI 0x20

#define IRQ_MASK 0x1
#define IRQ_UNMASK 0x0

void pic_disable(void);

#endif