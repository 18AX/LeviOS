#ifndef MEMFS_HEADER
#define MEMFS_HEADER

#include <levi/types.h>

#define MEMFS_MAX_FILENAME 128

typedef struct memfile
{
    char name[MEMFS_MAX_FILENAME]; // Use as a key
    u64 base_address;
    u64 size;
    u32 open_count;
    u32 flags;
} memfile_t;

STATUS memfs_init(void);

/**
 * @brief
 *
 * @param name
 * @param base_address
 * @param len
 * @param flags
 * @return STATUS
 */
STATUS memfile_create(const char *name, u64 base_address, u64 len, u32 flags);

STATUS memfile_destroy(const char *name);

#endif