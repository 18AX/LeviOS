#include "levi/proc/process.h"

#include "levi/memory/memory.h"
#include "levi/utils/string.h"

static proc_t *proc_list[MAX_PROCESS] = { NULL };

proc_t *process_create(const char name[PROCESS_NAME_LEN], proc_t *parent,
                       vas_t *vas)
{
    proc_t *proc = kmalloc(sizeof(proc_t));

    if (proc == NULL)
    {
        return NULL;
    }

    u64 len = strlen(name);

    u64 cpy_len = len > PROCESS_NAME_LEN ? PROCESS_NAME_LEN : len;

    memcpy(proc->name, name, cpy_len);

    proc->parent = parent;

    if (vas == NULL)
    {
        if (fill_empty_vas(&proc->vas) == FAILED)
        {
            kfree(proc);
        }
    }
    else
    {
        proc->vas = *vas;
    }

    u32 f = 0;

    for (u64 i = 0; i < MAX_PROCESS; ++i)
    {
        if (proc_list[i] == NULL)
        {
            proc->id = i;
            proc_list[i] = proc;
            f = 1;
            break;
        }
    }

    if (f == 0)
    {
        kfree(proc);
        return NULL;
    }

    for (u32 i = 0; i < FD_TABLE_LEN; ++i)
    {
        proc->fds[i] = NULL;
    }

    return proc;
}

void process_delete(proc_t *proc)
{
    if (proc == NULL || proc->id >= MAX_PROCESS)
    {
        return;
    }

    proc_list[proc->id] = NULL;
    kfree(proc);
}

proc_t *proc_get(u32 id)
{
    if (id >= MAX_PROCESS)
    {
        return NULL;
    }

    return proc_list[id];
}