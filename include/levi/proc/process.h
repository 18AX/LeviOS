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

typedef struct process
{
    u32 id;
    char name[PROCESS_NAME_LEN];
    vas_t vas;
    file_t *fds[FD_TABLE_LEN];
    u32 flags;
    context_t ctx;
} proc_t;

proc_t *proc_create(const char name[PROCESS_NAME_LEN], u32 flags);

/**
 * @brief Allocate n pages for the stack process.
 *
 * @param proc
 * @param address
 * @param nb_page
 * @return STATUS
 */
STATUS proc_allocate_stack(proc_t *proc, u64 address, u64 nb_page);

proc_t *proc_kernel(const char name[PROCESS_NAME_LEN], vas_t *vas);

void proc_delete(proc_t *proc);

proc_t *proc_get(u32 id);

#endif