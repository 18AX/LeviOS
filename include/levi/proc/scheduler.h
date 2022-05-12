#ifndef SCHEDULER_HEADER
#define SCHEDULER_HEADER

#include <levi/proc/process.h>

void sched_set(u32 id);

/**
 * @brief Get the process to run
 *
 * @return u32 the id of the process to run
 */
u32 sched_get(void);

#endif