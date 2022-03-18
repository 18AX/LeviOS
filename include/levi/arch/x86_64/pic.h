#ifndef PIC_HEADER
#define PIC_HEADER

#define PIC_MASTER_A 0x20
#define PIC_MASTER_B 0x21
#define PIC_SLAVE_A 0xA0
#define PIC_SLAVE_B 0xA1

#define PIC_MASTER_OFFSET 0x30
#define PIC_SLAVE_OFFSET 0x38

#define PIC_EOI 0x20

#define IRQ_MASK 0x1
#define IRQ_UNMASK 0x0

#include "levi/types.h"

void pic_init(void);

void pic_eoi(u8 pic);

void pic_set_irq(u8 pic, u8 irq, u8 state);

#endif