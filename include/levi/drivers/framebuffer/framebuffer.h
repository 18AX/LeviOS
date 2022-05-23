#ifndef FRAMEBUFFER_HEADER
#define FRAMEBUFFER_HEADER

#include <levi/stivale2.h>
#include <levi/types.h>

struct framebuffer_info
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
};

struct framebuffer_color
{
    u32 r;
    u32 g;
    u32 b;
};

STATUS
framebuffer_init(struct stivale2_struct *boot_info);

STATUS framebuffer_get_info(struct framebuffer_info *info);

u32 framebuffer_pixel_offset(u32 x, u32 y);

u32 framebuffer_pixel_color(struct framebuffer_color color);

#endif