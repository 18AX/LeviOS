#ifndef SCHEDULER_HEADER
#define SCHEDULER_HEADER

#include <levi/proc/process.h>
#include <levi/types.h>

STATUS sched_init(void);

STATUS sched_add(u64 id);

void sched_remove(u64 id);

/**
 * @brief Get the process to run
 *
 * @return u32 the id of the process to run
 */
u64 sched_get(void);

void sched_start(void);

#endif