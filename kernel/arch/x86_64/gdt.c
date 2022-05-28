#include <levi/arch/x86_64/gdt.h>

static struct gdt_entry gdt_entries[GDT_SIZE];
static struct gdt_ptr gdt_ptr = { .size = sizeof(struct gdt_entry) * GDT_SIZE,
                                  .offset = (u64)gdt_entries };

#if 0
__attribute__((naked))
static void reload_cs(void)
{
    asm volatile(
        "popq %rax\n" // get the return address
        "pushq $0x8\n"
        "pushq %rax\n"
        "lretq\n"
    );
}
#endif
struct gdt_entry gdt_entry_create(u8 access, u8 flags)
{
    return (struct gdt_entry){ .limit0 = 0xFFFF,
                               .limit1 = 0xF,
                               .base0 = 0x0,
                               .base1 = 0x0,
                               .base2 = 0x0,
                               .access_bytes = access,
                               .flags = flags & 0xF };
}

extern void __gdt_update_segments(void *ptr);

void gdt_init()
{
    /** Null descriptor **/
    gdt_entries[0].raw = 0x0;

    /** Kernel code **/
    gdt_entries[1] = gdt_entry_create(GDT_ACCESS_PRESENT | GDT_ACCESS_DPL(0)
                                          | GDT_ACCESS_TYPE_CODE_DATA
                                          | GDT_ACCESS_EXECUTABLE,
                                      GDT_FLAG_4KIB | GDT_FLAG_LONG);
    /** Kernel data **/
    gdt_entries[2] =
        gdt_entry_create(GDT_ACCESS_PRESENT | GDT_ACCESS_DPL(0)
                             | GDT_ACCESS_TYPE_CODE_DATA | GDT_ACCESS_RW,
                         GDT_FLAG_4KIB | GDT_FLAG_LONG);

    /** User code **/
    gdt_entries[3] = gdt_entry_create(GDT_ACCESS_PRESENT | GDT_ACCESS_DPL(3)
                                          | GDT_ACCESS_TYPE_CODE_DATA
                                          | GDT_ACCESS_EXECUTABLE,
                                      GDT_FLAG_4KIB | GDT_FLAG_4KIB);

    /** User data **/
    gdt_entries[4] =
        gdt_entry_create(GDT_ACCESS_PRESENT | GDT_ACCESS_DPL(3)
                             | GDT_ACCESS_TYPE_CODE_DATA | GDT_ACCESS_RW,
                         GDT_FLAG_4KIB | GDT_FLAG_4KIB);

    __gdt_update_segments(&gdt_ptr);
#if 0
    
    asm volatile(
        "pushq %rax\n"
        "callq reload_cs\n"
    );
#endif
}