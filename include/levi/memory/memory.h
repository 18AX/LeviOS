#ifndef MEMORY_HEADER
#define MEMORY_HEADER

#define PAGE_SIZE 4096

#include "levi/memory/vmm.h"
#include "levi/types.h"

#define KERNEL_ADDRESS 0xffffffff80000000
#define HHDM_ADDRESS 0xffff800000000000

#define GIB_4 4294967296
#define KERNEL_MAP_SIZE 0x80000000

STATUS memory_init(vas_t *kvas);

u64 align_up(u64 ptr);

u64 align_down(u64 ptr);

u32 is_align(u64 ptr);

u64 kaddress_to_phys(u64 addr);

u64 phys_to_kaddress(u64 addr);

u64 hhdm_to_phys(u64 addr);

u64 phys_to_hhdm(u64 addr);

#endif