#include <elf.h>
#include <levi/arch.h>
#include <levi/fs/file.h>
#include <levi/memory/memory.h>
#include <levi/memory/page_alloc.h>
#include <levi/proc/scheduler.h>
#include <levi/syscall/exec.h>
#include <levi/syscall/syscall.h>
#include <levi/utils/string.h>
#include <link.h>

u64 sys_exec(proc_t *proc, u64 args0, u64 args1, u64 args2, u64 args3)
{
    (void)args2;
    (void)args3;
    char path[1024];
    char proc_name[PROCESS_NAME_LEN];

    cpy_from_proc(proc, path, (void *)args0, 1024);
    cpy_from_proc(proc, proc_name, (void *)args1, PROCESS_NAME_LEN);

    s32 fd = kopen(path, FS_READ);

    if (fd < 0)
    {
        return SYSCALL_FAILED;
    }

    proc_t *n_proc = proc_create(proc_name, 0x0);

    if (n_proc == FAILED)
    {
        kclose(fd);
        return SYSCALL_FAILED;
    }
    ElfW(Ehdr) header;

    kread(fd, &header, sizeof(header));

    /** TODO: Check elf magic header **/

    klseek(fd, header.e_phoff, FS_SEEK_SET);

    for (u32 i = 0; i < header.e_phnum; ++i)
    {
        /** Move the cursor to the next program header **/
        klseek(fd, header.e_phoff + sizeof(ElfW(Phdr)) * i, FS_SEEK_SET);

        ElfW(Phdr) phdr;

        kread(fd, &phdr, sizeof(ElfW(Phdr)));

        if (phdr.p_type != PT_LOAD)
        {
            continue;
        }

        u64 nb_page = (phdr.p_memsz / PAGE_SIZE) + 1;

        /** TODO: change with an allocator that keep info on the process **/
        u64 addr = (u64)kframe_alloc(nb_page);
        memset((void *)addr, 0, phdr.p_memsz);

        if (addr == NULL)
        {
            kclose(fd);
            return SYSCALL_FAILED;
        }

        /** Set flags for the elf segment **/

        u32 flags = VM_PRESENT | VM_USER;

        /** segment not executable **/
        if ((phdr.p_flags & PF_X) == 0)
        {
            flags |= VM_EXECUTE_DISABLE;
        }

        if ((phdr.p_flags & PF_W) == 0)
        {
            flags |= VM_READ_WRITE;
        }

        if (vmmap_range(&n_proc->vas, hhdm_to_phys(addr), phdr.p_vaddr,
                        phdr.p_memsz, flags)
            == MAP_FAILED)
        {
            kframe_free((void *)addr, nb_page);
            kclose(fd);
            return SYSCALL_FAILED;
        }

        /** Copy segment to destination **/
        klseek(fd, phdr.p_offset, FS_SEEK_SET);
        kread(fd, (void *)addr, phdr.p_filesz);
    }

    /** allocate a stack of 8388608 bytes **/
    if (proc_allocate_stack(n_proc, USER_STACK_ADDRESS, USER_STACK_PAGE_NB)
        == FAILED)
    {
        return SYSCALL_FAILED;
    }

    arch_ctx_init(&n_proc->ctx, (void *)header.e_entry,
                  (void *)(USER_STACK_ADDRESS + PAGE_SIZE * USER_STACK_PAGE_NB),
                  0);

    sched_set(n_proc->id);
    kclose(fd);

    return n_proc->id;
}

u64 kexec(const char *path, const char *proc_name, const char *argv[],
          const char *envp[])
{
    proc_t *kernel_proc = proc_get(0);

    u64 id =
        sys_exec(kernel_proc, (u64)path, (u64)proc_name, (u64)argv, (u64)envp);

    if (id == SYSCALL_FAILED)
    {
        return FAILED;
    }

    proc_t *n_proc = proc_get(id);

    if (n_proc == NULL)
    {
        return FAILED;
    }

    switch_vas(&n_proc->vas);
    arch_ctx_set(&n_proc->ctx);

    return SUCCESS;
}

s64 exec(const char *path, const char *name, const char *argv[],
         const char *envp[]);

s64 exit(s64 exit_status);