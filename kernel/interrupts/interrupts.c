#include "levi/interrupts/interrupts.h"

#include "levi/panic.h"
#include "levi/stivale2.h"

static void default_handler(u64 id, u64 error_code, proc_t *proc)
{
    (void)id;
    (void)error_code;
    (void)proc;

    asm volatile("xchgw %bx, %bx");

    if (id < 30)
    {
        interrupts_disable();
        die();
    }
}

static interrupt_handler_t handlers[INTERRUPTS_NUMBER] = { 0x0 };

void throw_interrupts(u64 id, u64 error_code, proc_t *proc)
{
    handlers[id](id, error_code, proc);
}

void interrupt_init()
{
    for (u64 i = 0; i < INTERRUPTS_NUMBER; ++i)
    {
        handlers[i] = default_handler;
    }
}

STATUS register_interrupt_handler(u64 id, interrupt_handler_t h)
{
    if (id <= INTERRUPTS_NUMBER && handlers[id] == default_handler)
    {
        handlers[id] = h;
        return SUCCESS;
    }

    return FAILED;
}

STATUS unregister_interrupt_handler(u64 id)
{
    if (id <= INTERRUPTS_NUMBER && handlers[id] != default_handler)
    {
        handlers[id] = default_handler;
        return SUCCESS;
    }

    return FAILED;
}