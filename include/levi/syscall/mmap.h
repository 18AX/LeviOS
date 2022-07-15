#ifndef HEAP_ALLOC_HEADER
#define HEAP_ALLOC_HEADER

#include <levi/proc/process.h>
#include <levi/types.h>

#define MMAP_RDONLY 0x1
#define MMAP_WRONLY 0x2
#define MMAP_EXEC 0x3

u64 sys_mmap(proc_t *proc, u64 base_address, u64 nb_page, u64 flag, u64 unused);

#endif