#ifndef FS_HEADER
#define FS_HEADER

#include "levi/types.h"

#define FS_READ 0x0
#define FS_WRITE 0x1

#define FS_SEEK_SET 0
#define FS_SEEK_CUR 1
#define FS_SEEK_END 2

typedef struct file
{
    u32 flags;
    u64 cursor_position;
    struct vfs *vfs;
    u32 open_count;
    void *data;
} file_t;

struct vfs_operation
{
    struct node *(*mkdir)(const char *name, const char *path, u32 flags);
    s32 (*rmdir)(const char *path);
    s32 (*rename)(const char *path, const char *name);

    s32 (*read)(file_t *file, u8 *buffer, u32 size);
    s32 (*write)(file_t *file, u8 *buffer, u32 size);
    file_t *(*open)(const char *name, u32 flags);
    s32 (*flush)(file_t *file);
    s32 (*lseek)(file_t *file, u64 offset, u32 whence);

    void (*destroy_file)(file_t *file);
};

typedef struct vfs
{
    char *name; // Use as map key;
    u32 flags;

    struct vfs_operation *operation;
} vfs;

STATUS init_fs(void);

STATUS register_fs(struct vfs *vfs);

STATUS unregister_fs(struct vfs *vfs);

file_t *open_file(const char *path, u32 flags);

void destroy_file(file_t *file);

#endif