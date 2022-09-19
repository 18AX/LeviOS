#ifndef ARCH_HEADER
#define ARCH_HEADER

#include <levi/arch/x86_64/tss.h>
#include <levi/stivale2.h>
#include <levi/time/time.h>
#include <levi/types.h>

// Struct registers to be define by the arch

#if 1
#    define RFLAGS_CF (1)
#    define RFLAGS_RESERVED1 (1 << 1)
#    define RFLAGS_PF (1 << 2)
#    define RFLAGS_AF (1 << 4)
#    define RFLAGS_ZF (1 << 6)
#    define RFLAGS_SF (1 << 7)
#    define RFLAGS_OF (1 << 11)
#    define RFLAGS_DF (1 << 10)

#    define RFLAGS_IF (1 << 9)
#    define RFLAGS_IOPL (x)((x & 0x3) << 12)
#    define RFLAGS_NT (1 << 14)
#    define RFLAGS_RF (1 << 16)
#    define RFLAGS_VM (1 << 17)
#    define RFLAGS_AC (1 << 18)
#    define RFLAGS_VIF (1 << 19)
#    define RFLAGS_VIP (1 << 20)
#    define RFLAGS_ID (1 << 21)

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

struct isr_context
{
    regs_t regs;
    u64 index;
    u64 error_code;
    u64 rip;
    u64 cs;
    u64 rflags;
    u64 rsp;
    u64 ss;
} __attribute__((packed));

typedef struct context
{
    struct isr_context isr_ctx;
} context_t;

#endif

STATUS
arch_init(struct stivale2_struct *boot_info);

void arch_ctx_init(context_t *ctx, void *entry, void *stack, u8 is_kernel);

void arch_ctx_set(context_t *ctx);

void arch_set_timer(u64 ms);

void arch_get_time(struct time *time);

#endif