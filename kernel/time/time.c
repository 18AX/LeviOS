#include <levi/interrupts/interrupts.h>
#include <levi/time/time.h>

static u64 count = 0;

static void timer_handler(u64 id, u64 error_code, proc_t *proc)
{
    (void)id;
    (void)error_code;
    (void)proc;

    ++count;
}

u64 timer_init(void)
{
    return register_interrupt_handler(INTERRUPTS_TIMER_OFFSET, timer_handler);
}

u64 timer_count()
{
    return count;
}
