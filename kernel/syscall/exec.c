#include <elf.h>
#include <levi/arch.h>
#include <levi/fs/file.h>
#include <levi/memory/memory.h>
#include <levi/proc/scheduler.h>
#include <levi/syscall/exec.h>
#include <levi/syscall/syscall.h>
#include <levi/utils/kprintf.h>
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

    kprintf("Process allocated\n");

    ElfW(Ehdr) header;

    kread(fd, &header, sizeof(header));

    /** TODO: Check elf magic header **/

    klseek(fd, header.e_phoff, FS_SEEK_SET);

    for (u32 i = 0; i < header.e_phnum; ++i)
    {
        ElfW(Phdr) phdr;

        kread(fd, &phdr, sizeof(ElfW(Phdr)));

        if (phdr.p_type != PT_LOAD)
        {
            continue;
        }

        /** TODO: change with an allocator that keep info on the process **/
        u64 addr = (u64)kmalloc(phdr.p_memsz);

        if (addr == NULL)
        {
            return SYSCALL_FAILED;
        }

        u32 flags = VM_PRESENT;

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
            return SYSCALL_FAILED;
        }
    }

    kprintf("Segment mapped");

    /** allocate a stack of 8388608 bytes **/
    if (proc_allocate_stack(n_proc, 0x7ffffffff000, 2048) == FAILED)
    {
        return SYSCALL_FAILED;
    }

    kprintf("Stack allocated\n");

    arch_init_ctx(&n_proc->ctx, (void *)header.e_entry, (void *)0x7ffffffff000,
                  0);

    sched_set(n_proc->id);

    return n_proc->id;
}

u64 kexec(const char *path, const char *proc_name, const char *argv[],
          const char *envp[])
{
    return ksyscall(SYSCALL_EXEC, (u64)path, (u64)proc_name, (u64)argv,
                    (u64)envp);
}