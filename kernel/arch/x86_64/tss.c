#include <levi/arch/x86_64/tss.h>
#include <levi/syscall/syscall.h>
#include <levi/utils/kprintf.h>
#include <levi/utils/string.h>

struct task_state_segment tss = { 0 };

struct gdt_tss_entry tss_create_entry()
{
    memset(&tss, 0, sizeof(struct task_state_segment));
    tss.rsp[0] = (u64)syscall_stack_addr();
    tss.iopb = sizeof(struct task_state_segment);

    return (struct gdt_tss_entry){
        .base0 = ((u64)&tss) & 0xFFFF,
        .base1 = (((u64)&tss) >> 16) & 0xFF,
        .base2 = (((u64)&tss) >> 24) & 0xFF,
        .base3 = (((u64)&tss) >> 32) & 0xFFFFFFFF,
        .access_bytes = GDT_ACCESS_PRESENT | GDT_ACCESS_TSS_AVL
            | GDT_ACCESS_TYPE_SYSTEM | GDT_ACCESS_DPL(0),
        .limit0 = sizeof(struct task_state_segment) & 0xFF,
        .limit1 = (sizeof(struct task_state_segment) >> 16) & 0xF,
        .flags = GDT_FLAG_LONG,
        .reserved = 0x0
    };
}

void tss_ltr()
{
    asm volatile("movw $0x28,%%ax\n"
                 "ltr %%ax\n"
                 :
                 :
                 : "ax");
}