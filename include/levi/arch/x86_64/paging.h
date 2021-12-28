#ifndef PAGING_HEADER
#define PAGING_HEADER

#include "levi/types.h"

#define PML_SIZE 512

struct pml_entry
{
    union
    {
        struct
        {
            u8 present : 1;
            u8 read_write : 1;
            u8 user : 1;
            u8 cache : 1;
            u8 cache_disable : 1;
            u8 access : 1;
            u8 zero : 1;
            u8 page_size : 1;
            u8 ignored : 1;
            u8 available : 3;
            u64 page_table_addr : 52;
        } __attribute__((packed));

        struct
        {
            u64 raw;
        } __attribute__((packed));
    };

} __attribute__((packed));

#endif