#include "levi/proc/process.h"

#include "levi/memory/memory.h"
#include "levi/utils/string.h"

static proc_t *proc_list[MAX_PROCESS] = { NULL };

proc_t *process_create(const char name[PROCESS_NAME_LEN], proc_t *parent,
                       vas_t *vas, u32 flags)
{
    proc_t *proc = kmalloc(sizeof(proc_t));

    if (proc == NULL)
    {
        return NULL;
    }

    u64 len = strlen(name);

    if (len >= PROCESS_NAME_LEN)
    {
        kfree(proc);
        return NULL;
    }

    memcpy(proc->name, name, len);

    proc->parent = parent;

    // We need to initalize a new vas
    if (vas == NULL)
    {
        if (fill_empty_vas(&proc->vas) == FAILED)
        {
            kfree(proc);
        }

        // We copy the vas of the parent
        if (parent != NULL && (flags & PROCESS_SHARED_VAS) != 0)
        {
            vascpy(&proc->vas, &parent->vas);
        }
    }
    else
    {
        proc->vas = *vas;
    }

    u32 found_id = 0;

    for (u64 i = 0; i < MAX_PROCESS; ++i)
    {
        if (proc_list[i] == NULL)
        {
            proc->id = i;
            proc_list[i] = proc;
            found_id = 1;
            break;
        }
    }

    if (found_id == 0)
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

    destroy_vas(&proc->vas);

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