#ifndef PROCESS_HEADER
#define PROCESS_HEADER

#include "levi/types.h"

#define PROCESS_NAME_LEN 32
#define MAX_PROCESS 1024

typedef struct process
{
    u32 id;
    char name[PROCESS_NAME_LEN];
    struct process *parent;
} proc_t;

proc_t *process_create(const char name[PROCESS_NAME_LEN], proc_t *parent);

void process_delete(proc_t *proc);

proc_t *proc_get(u32 id);

#endif