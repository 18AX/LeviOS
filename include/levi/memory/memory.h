#ifndef MEMORY_HEADER
#define MEMORY_HEADER

#define PAGE_SIZE 4096

#include "levi/types.h"

u64 align_up(u64 ptr);

u64 align_down(u64 ptr);

u32 is_align(u64 ptr);

#endif