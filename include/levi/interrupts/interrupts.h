#ifndef INTERRUPTS_HEADER
#define INTERRUPTS_HEADER

#include "levi/arch.h"

#define INTERRUPTS_NUMBER 0xFF

typedef void (*interrupt_handler_t)(u64 id, u64 error_code, regs_t *regs);

void throw_interrupts(u64 id, u64 errror_code, regs_t *regs);

void interrupt_init(void);

STATUS register_interrupt_handler(u64 id, interrupt_handler_t func);

STATUS unregister_interrupt_handler(u64 id);

// Arch dependant
void interrupts_disable(void);

void interrupts_enable(void);

#endif