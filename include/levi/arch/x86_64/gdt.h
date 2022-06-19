#ifndef GDT_HEADER
#define GDT_HEADER

#include <levi/types.h>

#define GDT_SIZE 0x5

#define GDT_ACCESS_PRESENT (1 << 7)
#define GDT_ACCESS_DPL(x) ((x & 0x3) << 5)
#define GDT_ACCESS_TYPE_CODE_DATA (1 << 4)
#define GDT_ACCESS_TYPE_SYSTEM (0 << 4)
#define GDT_ACCESS_EXECUTABLE (1 << 3)
#define GDT_ACCESS_RW (1 << 1)

#define GDT_ACCESS_LDT 0x2
#define GDT_ACCESS_TSS_BSY 0xB
#define GDT_ACCESS_TSS_AVL 0x9

#define GDT_FLAG_4KIB (1 << 3)
#define GDT_FLAG_LONG (1 << 1)

#define SEGMENT_SELECTOR(index, ti, rpl)                                       \
    ((index << 3) | ((ti & 0x1) << 2) | (rpl & 0x3))

struct gdt_segment_entry
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

struct gdt_tss_entry
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
            u32 base3;
            u32 reserved;
        } __attribute__((packed));

        struct
        {
            u64 raw[2];
        } __attribute__((packed));
    };

} __attribute__((packed));

struct gdt
{
    struct gdt_segment_entry segments[GDT_SIZE];
    struct gdt_tss_entry tss;
} __attribute__((packed));

struct gdt_ptr
{
    u16 size;
    struct gdt *gdt;
} __attribute__((packed));

void gdt_init(void);

#endif