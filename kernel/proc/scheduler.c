#include <levi/proc/scheduler.h>

static u32 current_id = 0;

void sched_set(u32 id)
{
    current_id = id;
}

u32 sched_get(void)
{
    return current_id;
}