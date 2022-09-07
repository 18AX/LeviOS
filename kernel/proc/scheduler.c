#include <levi/arch.h>
#include <levi/interrupts/interrupts.h>
#include <levi/proc/scheduler.h>
#include <levi/utils/list.h>

static struct list_element *current = NULL;

static list_t *processes = NULL;

#include <levi/utils/kprintf.h>

static u64 i = 0;

static void timer_handler(u64 id, u64 error_code, proc_t *proc)
{
    (void)id;
    (void)error_code;
    (void)proc;
    kprintf("SCHEDULER HANDLER %lu\n", i++);

    if (current != NULL)
    {
        if (current->next != NULL)
        {
            current = current->next;
        }
    }

    arch_set_timer(200);
}

STATUS sched_init()
{
    processes = list_create();

    if (processes == NULL)
    {
        return FAILED;
    }

    register_interrupt_handler(INTERRUPTS_TIMER_OFFSET, timer_handler);

    return SUCCESS;
}

STATUS sched_add(u64 id)
{
    list_add(processes, (void *)id);

    if (current == NULL)
    {
        current = processes->head;
    }
    return SUCCESS;
}

void sched_remove(u64 id)
{
    list_remove_all(processes, (void *)id);
}

u64 sched_get(void)
{
    if (current == NULL)
    {
        return -1;
    }

    return (u64)current->data;
}

void sched_start()
{
    arch_set_timer(200);
}