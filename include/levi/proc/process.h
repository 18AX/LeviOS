#ifndef PROCESS_HEADER
#define PROCESS_HEADER

#include <levi/arch.h>
#include <levi/fs/fs.h>
#include <levi/memory/vmm.h>
#include <levi/types.h>

#define PROCESS_NAME_LEN 32
#define MAX_PROCESS 1024
#define FD_TABLE_LEN 128

#define PROCESS_KERNEL 0x1
#define PROCESS_SHARED_VAS 0x2

typedef struct process
{
    u32 id;
    char name[PROCESS_NAME_LEN];
    struct process *parent;
    vas_t vas;
    file_t *fds[FD_TABLE_LEN];
    context_t ctx;
} proc_t;

proc_t *process_create(const char name[PROCESS_NAME_LEN], proc_t *parent,
                       vas_t *vas, u32 flags);

void process_delete(proc_t *proc);

proc_t *proc_get(u32 id);

#endif