#include <levi/arch/x86_64/paging.h>
#include <levi/memory/memory.h>
#include <levi/memory/page_alloc.h>
#include <levi/memory/vmm.h>
#include <levi/utils/string.h>

STATUS switch_vas(vas_t *vas)
{
    u64 addr = hhdm_to_phys((u64)vas->data);

    asm volatile("movq %0,%%cr3\n" ::"a"(addr));

    return SUCCESS;
}

vas_t get_vas(void)
{
    u64 value = 0;

    asm volatile("movq %%cr3,%0" : "=r"(value));

    return (vas_t){ (struct pml_entry *)phys_to_hhdm(value) };
}

static struct pml_entry *init_pml(void)
{
    u8 *pd = kframe_alloc(1);

    if (pd == NULL)
    {
        return NULL;
    }

    for (u32 i = 0; i < PAGE_SIZE; ++i)
    {
        pd[i] = 0;
    }

    return (struct pml_entry *)pd;
}

STATUS fill_empty_vas(vas_t *vas)
{
    struct pml_entry *entry = init_pml();

    if (entry == NULL)
    {
        return FAILED;
    }

    vas->data = entry;

    return SUCCESS;
}

static struct pml_entry *find_or_allocate_pml(struct pml_entry *pml, u64 idx,
                                              u64 flags)
{
    if (pml[idx].present == 1)
    {
        return (struct pml_entry *)(phys_to_hhdm(
            LSHIFT((u64)pml[idx].page_table_addr, 12)));
    }

    struct pml_entry *n_pml = init_pml();

    if (n_pml == NULL)
    {
        return NULL;
    }

    pml[idx].raw = flags | ((hhdm_to_phys((u64)n_pml)) & ~(0xFFF));

    return n_pml;
}

static struct pml_entry *find_page(vas_t *vm, u64 virt)
{
    struct pml_entry *pml4 = (struct pml_entry *)vm->data;

    u64 pml3_idx = (virt >> 39) & 0x1FF;

    u64 pml2_idx = (virt >> 30) & 0x1FF;

    u64 pml1_idx = (virt >> 21) & 0x1FF;

    u64 pml0_idx = (virt >> 12) & 0x1FF;

    if (pml4[pml3_idx].present == 0)
    {
        return NULL;
    }

    struct pml_entry *pdpt = (struct pml_entry *)phys_to_hhdm(
        LSHIFT((u64)pml4[pml3_idx].page_table_addr, 12));

    if (pdpt[pml2_idx].present == 0)
    {
        return NULL;
    }

    struct pml_entry *pd = (struct pml_entry *)phys_to_hhdm(
        LSHIFT((u64)pdpt[pml2_idx].page_table_addr, 12));

    if (pd[pml1_idx].present == 0)
    {
        return NULL;
    }

    struct pml_entry *pt = (struct pml_entry *)phys_to_hhdm(
        LSHIFT((u64)pd[pml1_idx].page_table_addr, 12));

    return &pt[pml0_idx];
}

MAP_STATUS vmmap(vas_t *vas, u64 physical, u64 virt, u64 flags)
{
    if (vas == NULL)
    {
        return MAP_FAILED;
    }

    struct pml_entry *pml4 = (struct pml_entry *)vas->data;

    u64 pml3_idx = (virt >> 39) & 0x1FF;

    u64 pml2_idx = (virt >> 30) & 0x1FF;

    u64 pml1_idx = (virt >> 21) & 0x1FF;

    u64 pml0_idx = (virt >> 12) & 0x1FF;

    struct pml_entry *pdpt = find_or_allocate_pml(
        pml4, pml3_idx, VM_PRESENT | VM_READ_WRITE | VM_USER);

    if (pdpt == NULL)
    {
        return MAP_FAILED;
    }

    struct pml_entry *pd = find_or_allocate_pml(
        pdpt, pml2_idx, VM_PRESENT | VM_READ_WRITE | VM_USER);

    if (pd == NULL)
    {
        return MAP_FAILED;
    }

    struct pml_entry *pt = find_or_allocate_pml(
        pd, pml1_idx, VM_PRESENT | VM_READ_WRITE | VM_USER);

    if (pt == NULL)
    {
        return MAP_FAILED;
    }

    if (pt[pml0_idx].present == 1)
    {
        if (((u64)pt[pml0_idx].page_table_addr) == physical)
        {
            return MAP_PRESENT;
        }

        return MAP_FAILED;
    }

    pt[pml0_idx].raw = flags | VM_PRESENT | (physical & ~(0xFFF));

    return MAP_SUCCESS;
}

MAP_STATUS vmmap_range(vas_t *vas, u64 physical, u64 virt, u64 size, u64 flags)
{
    for (u64 i = 0; i < size; i += PAGE_SIZE)
    {
        if (vmmap(vas, physical + i, virt + i, flags) == MAP_FAILED)
        {
            return MAP_FAILED;
        }
    }

    return MAP_SUCCESS;
}

MAP_STATUS vmunmap(vas_t *vas, u64 virt)
{
    struct pml_entry *pt = find_page(vas, virt);

    if (pt == NULL)
    {
        return MAP_FAILED;
    }

    *pt = (struct pml_entry){ 0x0 };

    return MAP_SUCCESS;
}
MAP_STATUS vmunmap_range(vas_t *vas, u64 virt, u64 size)
{
    for (u64 i = 0; i < size; i += PAGE_SIZE)
    {
        if (vmunmap(vas, virt + i) == MAP_SUCCESS)
        {
            return MAP_FAILED;
        }
    }

    return MAP_SUCCESS;
}

MAP_STATUS vma_to_phys(vas_t *vas, u64 virt, u64 *phys)
{
    struct pml_entry *page = find_page(vas, virt);

    if (page == NULL)
    {
        return MAP_FAILED;
    }

    *phys = LSHIFT(page->page_table_addr, 12) | (virt & 0xFFF);

    return MAP_SUCCESS;
}

MAP_STATUS vascpy(vas_t *dst, vas_t *src)
{
    memcpy(dst->data, src->data, PAGE_SIZE);

    return MAP_SUCCESS;
}

static void destroy_vas_rec(struct pml_entry *entry)
{
    if (entry == NULL)
    {
        return;
    }

    for (u64 i = 0; i < PAGE_SIZE / sizeof(struct pml_entry); ++i)
    {
        destroy_vas_rec((struct pml_entry *)(phys_to_hhdm(
            (u64)LSHIFT(entry[i].page_table_addr, 12))));
    }

    kframe_free(entry, 1);
}

void destroy_vas(vas_t *vas)
{
    struct pml_entry *pml = vas->data;

    destroy_vas_rec(pml);

    kfree(vas);
}