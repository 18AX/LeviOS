#include <levi/arch.h>
#include <levi/interrupts/interrupts.h>
#include <levi/proc/scheduler.h>
#include <levi/time/time.h>
#include <levi/utils/linked_list.h>

static struct linked_list_elt *current = NULL;

static linked_list_t *processes = NULL;
static u64 to_count = 0;

void sched_schedule()
{
    u64 count = timer_count();

    if (count < to_count)
    {
        return;
    }

    if (current != NULL)
    {
        if (current->next != NULL)
        {
            current = current->next;
        }
    }

    to_count = count + 200;
}

STATUS sched_init()
{
    processes = linked_list_new(NULL);

    if (processes == NULL)
    {
        return FAILED;
    }

    return SUCCESS;
}

STATUS sched_add(u64 id)
{
    linked_list_add(processes, (void *)id);

    if (current == NULL)
    {
        current = processes->head;
    }
    return SUCCESS;
}

void sched_remove(u64 id)
{
    linked_list_remove_ptr(processes, (void *)id);
}

u64 sched_get(void)
{
    if (current == NULL)
    {
        return -1;
    }

    return (u64)current->value;
}