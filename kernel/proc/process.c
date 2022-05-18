#include <levi/memory/memory.h>
#include <levi/memory/page_alloc.h>
#include <levi/proc/process.h>
#include <levi/utils/string.h>

static proc_t *proc_list[MAX_PROCESS] = { NULL };

proc_t *proc_kernel(const char name[PROCESS_NAME_LEN], vas_t *vas)
{
    proc_t *proc = kmalloc(sizeof(proc_t));

    if (proc == NULL)
    {
        return NULL;
    }

    strncpy(proc->name, name, PROCESS_NAME_LEN);
    proc->flags = PROCESS_KERNEL;

    proc->vas = *vas;

    /** Intialize files descriptors array **/

    for (u32 i = 0; i < FD_TABLE_LEN; ++i)
    {
        proc->fds[i] = NULL;
    }

    proc_list[0] = proc;

    return proc;
}

proc_t *proc_create(const char name[PROCESS_NAME_LEN], u32 flags)
{
    proc_t *proc = kmalloc(sizeof(proc_t));

    if (proc == NULL)
    {
        return NULL;
    }

    STATUS found = FAILED;

    /** The first entry is reserved for the kernel process **/
    for (u32 i = 1; i < MAX_PROCESS; ++i)
    {
        if (proc_list[i] != NULL)
        {
            proc_list[i] = proc;
            found = SUCCESS;
            break;
        }
    }

    if (found == FAILED)
    {
        kfree(proc);
        return NULL;
    }

    strncpy(proc->name, name, PROCESS_NAME_LEN);
    proc->flags = flags;

    /** Initialize process virtual address space **/
    if (fill_empty_vas(&proc->vas) == FAILED)
    {
        kfree(proc);
        return NULL;
    }

    /** We need to copy kernel vas **/

    proc_t *kernel_proc = proc_get(0);

    if (kernel_proc == NULL)
    {
        kfree(proc);
        return NULL;
    }

    vascpy(&proc->vas, &kernel_proc->vas);

    /** Intialize files descriptors array **/

    for (u32 i = 0; i < FD_TABLE_LEN; ++i)
    {
        proc->fds[i] = NULL;
    }

    return proc;
}

STATUS proc_allocate_stack(proc_t *proc, u64 virt_address, u64 nb_page)
{
    if (proc == NULL)
    {
        return FAILED;
    }

    u64 addr = (u64)kframe_alloc(nb_page);

    if (addr == NULL)
    {
        return NULL;
    }

    u32 vmmap_flags = VM_READ_WRITE;

    if ((proc->flags & PROCESS_KERNEL) == 0)
    {
        vmmap_flags |= VM_USER;
    }

    if (vmmap_range(&proc->vas, addr, virt_address, PAGE_SIZE * nb_page,
                    vmmap_flags)
        == MAP_FAILED)
    {
        kframe_free((void *)addr, nb_page);
    }

    /** Update stacks registers **/
#if x86_64
    proc->ctx.rsp = virt_address;
    proc->ctx.regs.rbp = virt_address;
#endif

    return SUCCESS;
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