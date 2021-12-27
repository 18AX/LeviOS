#ifndef PAGE_ALLOC
#define PAGE_ALLOC

#include "levi/types.h"

struct free_list
{
    u64 size; // Size in number of pages
    struct free_list *next;
};

STATUS kframe_init(struct stivale2_struct *boot_info);

void *kframe_alloc(u64 pages);

void kframe_free(void *ptr, u64 pages);

#endif