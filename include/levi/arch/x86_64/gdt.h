#ifndef GDT_HEADER
#define GDT_HEADER

#include <levi/types.h>

#define GDT_SIZE 0x5

#define GDT_ACCESS_PRESENT (1 << 7)
#define GDT_ACCESS_DPL(x) ((x & 0x3) << 5)
#define GDT_ACCESS_TYPE_CODE_DATA (1 << 4)
#define GDT_ACCESS_EXECUTABLE (1 << 3)
#define GDT_ACCESS_RW (1 << 1)

#define GDT_FLAG_4KIB (1 << 3)
#define GDT_FLAG_LONG (1 << 1)

#define SEGMENT_SELECTOR(index, ti, rpl)                                       \
    ((index << 3) | ((ti & 0x1) << 2) | (rpl & 0x3))

struct gdt_entry
{
    union
    {
        struct
        {
            u16 limit0;
            u16 base0;
            u8 base1;
            u8 access_bytes;
            u8 limit1 : 4;
            u8 flags : 4;
            u8 base2;
        } __attribute__((packed));

        struct
        {
            u64 raw;
        } __attribute__((packed));
    };

} __attribute__((packed));

struct gdt_ptr
{
    u16 size;
    u64 offset;
} __attribute__((packed));

struct task_state_segment
{
} __attribute__((packed));

struct gdt_entry gdt_entry_create(u8 access, u8 flags);

void gdt_init(void);

#endif