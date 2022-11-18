#include <levi/boot/boot.h>
#include <levi/utils/string.h>

STATUS boot_info_from_stivale2(struct stivale2_struct *stivale2_struct,
                               struct boot_info *boot_info)
{
    // Retrieve the stivale2 structures we need
    struct stivale2_struct_tag_memmap *memmap =
        stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MEMMAP_ID);

    struct stivale2_struct_tag_modules *modules =
        stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_MODULES_ID);

    struct stivale2_struct_tag_framebuffer *framebuffer =
        stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

    struct stivale2_struct_tag_rsdp *rsdp =
        stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_RSDP_ID);

    if (memmap == NULL || modules == NULL || framebuffer == NULL
        || rsdp == NULL)
    {
        return FAILED;
    }

    // Copy the memory map entries
    u64 i = 0;
    for (; i < memmap->entries && i < MMAP_ENTRIES_MAX_LEN; ++i)
    {
        boot_info->mmap[i].base = memmap->memmap[i].base;
        boot_info->mmap[i].len = memmap->memmap[i].length;

        if (memmap->memmap[i].type == STIVALE2_MMAP_USABLE)
        {
            boot_info->mmap[i].type = MEM_USABLE;
        }
    }

    boot_info->mmap_len = i;

    // Copy the modules entries
    for (i = 0; i < modules->module_count; ++i)
    {
        u64 len = modules->modules[i].end - modules->modules[i].begin;

        boot_info->modules[i].base = modules->modules[i].begin;
        boot_info->modules[i].len = len;

        strncpy(boot_info->modules[i].name,
                modules->modules[i].string,
                MODULE_NAME_LEN);
    }

    boot_info->modules_len = i;

    // Copy information relative to the framebuffer
    boot_info->frammebufer_info.blue_mask_shift = framebuffer->blue_mask_shift;
    boot_info->frammebufer_info.blue_mask_size = framebuffer->blue_mask_size;
    boot_info->frammebufer_info.green_mask_shift =
        framebuffer->green_mask_shift;
    boot_info->frammebufer_info.green_mask_size = framebuffer->green_mask_size;
    boot_info->frammebufer_info.red_mask_shift = framebuffer->red_mask_shift;
    boot_info->frammebufer_info.red_mask_size = framebuffer->red_mask_size;
    boot_info->frammebufer_info.bpp = framebuffer->framebuffer_bpp;
    boot_info->frammebufer_info.height = framebuffer->framebuffer_height;
    boot_info->frammebufer_info.width = framebuffer->framebuffer_width;
    boot_info->frammebufer_info.pitch = framebuffer->framebuffer_pitch;
    boot_info->frammebufer_info.addr = framebuffer->framebuffer_addr;

    boot_info->rsdp = rsdp->rsdp;

    return SUCCESS;
}