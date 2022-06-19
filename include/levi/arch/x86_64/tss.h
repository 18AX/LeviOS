#ifndef TSS_HEADER
#define TSS_HEADER

#include <levi/arch/x86_64/gdt.h>
#include <levi/types.h>

struct task_state_segment
{
    u32 reserved0;
    u64 rsp[3];
    u64 reserved1;
    u64 ist[7];
    u64 reserved2;
    u16 reserved3;
    u16 iopb;
} __attribute__((packed));

struct gdt_tss_entry tss_create_entry(void);

#endif