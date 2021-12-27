#include "levi/memory/page_alloc.h"

#include "levi/memory/memory.h"

static struct free_list *free_list_entry = NULL;

STATUS kframe_init(struct stivale2_struct *boot_info)
{
    struct stivale2_struct_tag_memmap *memmap =
        stivale2_get_tag(boot_info, STIVALE2_STRUCT_TAG_MEMMAP_ID);

    if (memmap == NULL)
    {
        return FAILED;
    }

    struct stivale2_mmap_entry *mmap_entries = memmap->memmap;

    struct free_list *first = NULL;
    struct free_list *tmp = NULL;

    for (u64 i = 0; i < memmap->entries; ++i)
    {
        if (mmap_entries[i].type == STIVALE2_MMAP_USABLE)
        {
            u64 base_address = is_align(mmap_entries[i].base)
                ? mmap_entries[i].base
                : align_up(mmap_entries[i].base);

            u64 up_address =
                is_align(mmap_entries[i].base + mmap_entries[i].length)
                ? mmap_entries[i].base + mmap_entries[i].length
                : align_down(mmap_entries[i].base + mmap_entries[i].length);

            if (up_address <= base_address)
            {
                continue;
            }

            u64 max_page = (up_address - base_address) / PAGE_SIZE;

            if (max_page == 0)
            {
                continue;
            }

            struct free_list *mem_zone = (struct free_list *)base_address;

            mem_zone->size = max_page;
            mem_zone->next = NULL;

            if (tmp == NULL)
            {
                first = mem_zone;
                tmp = mem_zone;
            }
            else
            {
                tmp->next = mem_zone;
                tmp = mem_zone;
            }
        }
    }

    free_list_entry = first;

    return SUCCESS;
}

static struct free_list *split_block(struct free_list *block, u64 size)
{
    if (block->size == size)
    {
        return block;
    }

    struct free_list *n =
        (struct free_list *)(((char *)block) + PAGE_SIZE * size);

    n->next = block->next;
    n->size = block->size - size;

    block->size = size;
    block->next = n;

    return block;
}

static void merge_list(struct free_list *low, struct free_list *high)
{
    low->size += high->size;
    low->next = high->next;
}

void *kframe_alloc(u64 pages)
{
    if (free_list_entry == NULL)
    {
        return NULL;
    }

    struct free_list *prev = NULL;
    struct free_list *current = free_list_entry;

    while (current != NULL)
    {
        if (current->size >= pages)
        {
            struct free_list *alloc = split_block(current, pages);

            if (prev == NULL)
            {
                free_list_entry = alloc->next;
            }
            else
            {
                prev->next = alloc->next;
            }

            return (void *)alloc;
        }

        prev = current;
        current = current->next;
    }

    return NULL;
}

void kframe_free(void *ptr, u64 pages)
{
    if (ptr == NULL)
    {
        return;
    }

    if (free_list_entry == NULL)
    {
        struct free_list *n = (struct free_list *)ptr;
        n->next = NULL;
        n->size = pages;

        free_list_entry = n;
        return;
    }

    struct free_list *prev = NULL;
    struct free_list *current = free_list_entry;

    while (current != NULL)
    {
        if (((void *)current) > ptr)
        {
            break;
        }

        prev = current;
        current = current->next;
    }

    struct free_list *n = (struct free_list *)ptr;

    n->size = pages;
    n->next = current;

    if (prev == NULL)
    {
        free_list_entry = n;
    }
    else
    {
        prev->next = n;
    }

    if (((char *)ptr) + pages * PAGE_SIZE == ((char *)current))
    {
        merge_list(n, current);
    }

    if (prev != NULL
        && ((char *)prev) + prev->size * PAGE_SIZE == ((char *)ptr))
    {
        merge_list(prev, n);
    }
}

void kframe_dump(void)
{
    struct free_list *current = free_list_entry;

    term_print("----------------------------Memory "
               "dump----------------------------\n");
    while (current != NULL)
    {
        term_print("base address 0x%p size %lu\n", current,
                   current->size * PAGE_SIZE);

        current = current->next;
    }

    term_print("---------------------------------------------------------------"
               "----\n");
}