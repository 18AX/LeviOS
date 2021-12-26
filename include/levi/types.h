#ifndef TYPES_HEADER
#define TYPES_HEADER

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

#define STATUS u32

#define SUCCESS 1
#define FAILED 0

#ifndef NULL
#    define NULL 0x0
#endif
#endif