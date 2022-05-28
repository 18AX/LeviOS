#ifndef ARCH_HEADER
#define ARCH_HEADER

#include "levi/stivale2.h"
#include "levi/types.h"

// Struct registers to be define by the arch

#if 1

typedef struct registers
{
    u64 rax;
    u64 rbx;
    u64 rcx;
    u64 rdx;
    u64 rsi;
    u64 rdi;
    u64 r8;
    u64 r9;
    u64 r10;
    u64 r11;
    u64 r12;
    u64 r13;
    u64 r14;
    u64 r15;
    u64 rbp;
} __attribute__((packed)) regs_t;

typedef struct context
{
    regs_t regs;
    u64 index;
    u64 error_code;
    u64 rip;
    u64 cs;
    u64 eflags;
    u64 rsp;
    u64 ss;
} __attribute__((packed)) context_t;

#endif

STATUS
arch_init(struct stivale2_struct *boot_info);

void arch_init_ctx(context_t *ctx, void *entry, void *stack, u8 is_kernel);

#endif