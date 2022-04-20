#ifndef IDT_HEADER
#define IDT_HEADER

#include "levi/arch.h"
#include "levi/types.h"

#define INTERRUPT_GATE 0x8E
#define TRAP_GATE 0xF

struct interrupt_context
{
    regs_t regs;
    u64 index;
    u64 error_code;
    u64 rip;
    u64 cs;
} __attribute__((packed));

struct idt_64_entry
{
    u16 offset_1;
    u16 selector;
    u8 ist;
    u8 type_attr;
    u16 offset_2;
    u32 offset_3;
    u32 zero;
} __attribute__((packed));

struct idt_64_ptr
{
    u16 size;
    u64 offset;
} __attribute__((packed));

void idt_init(void);

void __isr_c_handler(struct interrupt_context *ctx);

#endif