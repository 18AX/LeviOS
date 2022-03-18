#ifndef ARCH_HEADER
#define ARCH_HEADER

#include "levi/stivale2.h"
#include "levi/types.h"

// Struct registers to ne define by the arch

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
} __attribute__((packed)) regs_t;

STATUS arch_init(struct stivale2_struct *boot_info);

#endif