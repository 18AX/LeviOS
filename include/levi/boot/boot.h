#ifndef BOOT_HEADER
#define BOOT_HEADER

#include <levi/boot/stivale2.h>
#include <levi/types.h>

/**
 * Levi defines its own boot structure to be compatible with different boot
 * protocol. Different boot protocols are tranlsated into levi_boot structure.
 */

#define MEM_USABLE 0x1

struct boot_mem_entry
{
    u64 base;
    u64 len;
    u32 type;
};

#define MODULE_NAME_LEN 128

struct boot_module_entry
{
    u64 base;
    u64 len;
    char name[MODULE_NAME_LEN];
};

struct boot_framebuffer_info
{
    u16 width;
    u16 height;
    u16 pitch;
    u16 bpp;
    u8 red_mask_size;
    u8 red_mask_shift;
    u8 green_mask_size;
    u8 green_mask_shift;
    u8 blue_mask_size;
    u8 blue_mask_shift;
    u64 addr;
};

#define MMAP_ENTRIES_MAX_LEN 0xFF
#define MODULE_ENTIRES_MAX_LEN 0xFF

struct boot_info
{
    struct boot_mem_entry mmap[MMAP_ENTRIES_MAX_LEN];
    u32 mmap_len;
    struct boot_module_entry modules[MODULE_ENTIRES_MAX_LEN];
    u32 modules_len;
    struct boot_framebuffer_info frammebufer_info;
    u64 rsdp;
};

STATUS boot_info_from_stivale2(struct stivale2_struct *stivale2_struct,
                               struct boot_info *boot_info);

#endif