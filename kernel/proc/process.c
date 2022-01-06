#include "levi/proc/process.h"

#include "levi/memory/memory.h"
#include "levi/utils/string.h"

static proc_t *proc_list[MAX_PROCESS] = { NULL };

proc_t *process_create(const char name[PROCESS_NAME_LEN], proc_t *parent)
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

    for (u64 i = 0; i < MAX_PROCESS; ++i)
    {
        if (proc_list[i] == NULL)
        {
            proc->id = i;
            proc_list[i] = proc;
            break;
        }
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