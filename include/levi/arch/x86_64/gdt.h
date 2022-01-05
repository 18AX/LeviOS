#ifndef GDT_HEADER
#define GDT_HEADER

#define SEGMENT_SELECTOR(index, ti, rpl)                                       \
    ((index << 3) | ((ti & 0x1) << 2) | (rpl & 0x3))

#endif