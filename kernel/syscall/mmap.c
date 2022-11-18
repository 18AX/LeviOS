#include <levi/memory/memory.h>
#include <levi/memory/page_alloc.h>
#include <levi/syscall/mmap.h>
#include <levi/syscall/syscall.h>

static u64 get_vmmap_flag(u64 mmap_flag)
{
    u64 flag = VM_USER;

    if ((mmap_flag & MMAP_WRONLY) != 0)
    {
        flag |= VM_READ_WRITE;
    }

    if ((mmap_flag & MMAP_EXEC) == 0)
    {
        flag |= VM_EXECUTE_DISABLE;
    }

    return flag;
}

u64 sys_mmap(proc_t *proc, u64 base_address, u64 nb_page, u64 flag, u64 unused)
{
    (void)unused;

    u64 ptr = (u64)kframe_alloc(nb_page);

    if (ptr == NULL)
    {
        return SYSCALL_FAILED;
    }

    /** Get the physical address **/
    u64 phys = hhdm_to_phys(ptr);

    u64 virt = base_address;

    if (virt == NULL)
    {
        virt = proc->end_code + phys;
    }

    if (vmmap_range(
            &proc->vas, phys, virt, PAGE_SIZE * nb_page, get_vmmap_flag(flag))
        == MAP_FAILED)
    {
        kframe_free((void *)ptr, nb_page);
        return SYSCALL_FAILED;
    }

    return virt;
}