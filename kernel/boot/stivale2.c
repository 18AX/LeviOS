#include "levi/stivale2.h"

#include "levi/utils/stdio.h"
#include "levi/utils/string.h"

static u8 stack[1048576]; // 1 Mib

// this come frome https://wiki.osdev.org/Stivale_Bare_Bones

static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    // Same as above.
    .tag = { .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
             // Instead of 0, we now point to the previous header tag. The order
             // in
             // which header tags are linked does not matter.
             .next = NULL },
    // We set all the framebuffer specifics to 0 as we want the bootloader
    // to pick the best it can.
    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp = 0
};

__attribute__((section(".stivale2hdr"),
               used)) static struct stivale2_header stivale_hdr = {
    // The entry_point member is used to specify an alternative entry
    // point that the bootloader should jump to instead of the executable's
    // ELF entry point. We do not care about that so we leave it zeroed.
    .entry_point = 0,
    // Let's tell the bootloader where our stack is.
    // We need to add the sizeof(stack) since in x86(_64) the stack grows
    // downwards.
    .stack = (u64)(stack + sizeof(stack)),
    // Bit 1, if set, causes the bootloader to return to us pointers in the
    // higher half, which we likely want.
    .flags = POINTER_HHDM | FULL_VIRTUAL_KERNEL, //(1 << 1),
    // This header structure is the root of the linked list of header tags and
    // points to the first one in the linked list.
    .tags = (u64)&framebuffer_hdr_tag
};

void *stivale2_get_tag(struct stivale2_struct *stivale2_struct, u64 id)
{
    struct stivale2_tag *current_tag = (void *)stivale2_struct->tags;
    for (;;)
    {
        // If the tag pointer is NULL (end of linked list), we did not find
        // the tag. Return NULL to signal this.
        if (current_tag == NULL)
        {
            return NULL;
        }

        // Check whether the identifier matches. If it does, return a pointer
        // to the matching tag.
        if (current_tag->identifier == id)
        {
            return current_tag;
        }

        // Get a pointer to the next tag in the linked list and repeat.
        current_tag = (void *)current_tag->next;
    }
}

static void (*term_write)(const char *string, u64 length) = NULL;

STATUS term_init(struct stivale2_struct *stivale2_struct)
{
    struct stivale2_struct_tag_terminal *term_str_tag =
        stivale2_get_tag(stivale2_struct, STIVALE2_STRUCT_TAG_TERMINAL_ID);

    if (term_str_tag == NULL)
    {
        return FAILED;
    }

    term_write = (void *)term_str_tag->term_write;

    return SUCCESS;
}