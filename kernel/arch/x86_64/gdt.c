#include <levi/arch/x86_64/gdt.h>
#include <levi/arch/x86_64/tss.h>

static struct gdt gdt = { 0 };
static struct gdt_ptr gdt_ptr = { .size = sizeof(struct gdt), .gdt = &gdt };

extern void __gdt_update_segments(void *ptr);

static struct gdt_segment_entry gdt_entry_create(u8 access, u8 flags)
{
    return (struct gdt_segment_entry){ .limit0 = 0xFFFF,
                                       .limit1 = 0xF,
                                       .base0 = 0x0,
                                       .base1 = 0x0,
                                       .base2 = 0x0,
                                       .access_bytes = access,
                                       .flags = flags & 0xF };
}

void gdt_init()
{
    /** Null descriptor **/
    gdt.segments[0].raw = 0x0;

    /** Kernel code **/
    gdt.segments[1] = gdt_entry_create(GDT_ACCESS_PRESENT | GDT_ACCESS_DPL(0)
                                           | GDT_ACCESS_TYPE_CODE_DATA
                                           | GDT_ACCESS_EXECUTABLE,
                                       GDT_FLAG_4KIB | GDT_FLAG_LONG);
    /** Kernel data **/
    gdt.segments[2] =
        gdt_entry_create(GDT_ACCESS_PRESENT | GDT_ACCESS_DPL(0)
                             | GDT_ACCESS_TYPE_CODE_DATA | GDT_ACCESS_RW,
                         GDT_FLAG_4KIB | GDT_FLAG_LONG);

    /** User code **/
    gdt.segments[3] = gdt_entry_create(GDT_ACCESS_PRESENT | GDT_ACCESS_DPL(3)
                                           | GDT_ACCESS_TYPE_CODE_DATA
                                           | GDT_ACCESS_EXECUTABLE,
                                       GDT_FLAG_4KIB | GDT_FLAG_4KIB);

    /** User data **/
    gdt.segments[4] =
        gdt_entry_create(GDT_ACCESS_PRESENT | GDT_ACCESS_DPL(3)
                             | GDT_ACCESS_TYPE_CODE_DATA | GDT_ACCESS_RW,
                         GDT_FLAG_4KIB | GDT_FLAG_4KIB);

    gdt.tss = tss_create_entry();

    __gdt_update_segments(&gdt_ptr);
}