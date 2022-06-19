#include <levi/arch/x86_64/tss.h>

struct task_state_segment tss = { 0 };

struct gdt_tss_entry tss_create_entry()
{
    return (struct gdt_tss_entry){
        .base0 = ((u64)&tss) & 0xFF,
        .base1 = (((u64)&tss) << 16) & 0xF,
        .base2 = (((u64)&tss) << 24) & 0xF,
        .base3 = (((u64)&tss) << 32) & 0xFFFF,
        .access_bytes = GDT_ACCESS_PRESENT | GDT_ACCESS_TSS_AVL
            | GDT_ACCESS_TYPE_SYSTEM | GDT_ACCESS_DPL(0),
        .limit0 = sizeof(struct task_state_segment),
        .limit1 = (sizeof(struct task_state_segment) << 16) & 0xF,
        .flags = 0x0
    };
}