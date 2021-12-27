#include "levi/memory/memory.h"

u64 align_up(u64 ptr)
{
    return (ptr + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);
}

u64 align_down(u64 ptr)
{
    return ptr & ~(PAGE_SIZE - 1);
}

u32 is_align(u64 ptr)
{
    return (ptr & (PAGE_SIZE - 1)) == 0;
}