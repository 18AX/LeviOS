#include <levi/fs/fs.h>
#include <levi/fs/memfs.h>
#include <levi/memory/memory.h>
#include <levi/utils/string.h>
#include <levi/utils/stringmap.h>

static file_t *__open(const char *name, u32 flags);
static void __destroy_file(file_t *file);
static s32 __write(file_t *file, u8 *buffer, u32 size);
static s32 __read(file_t *file, u8 *buffer, u32 size);
static s32 __lseek(file_t *file, u64 offset, u32 whence);

static struct vfs_operation memfs_operation = { .mkdir = NULL,
                                                .rmdir = NULL,
                                                .rename = NULL,
                                                .open = __open,
                                                .write = __write,
                                                .destroy_file = __destroy_file,
                                                .flush = NULL,
                                                .read = __read,
                                                .lseek = __lseek };

static struct vfs memfs = { .name = "memfs",
                            .flags = 0x0,
                            .operation = &memfs_operation };

static map_t memfiles_map = NULL;

STATUS memfs_init(void)
{
    memfiles_map = hashmap_new();

    if (memfiles_map == NULL)
    {
        return FAILED;
    }

    STATUS res = register_fs(&memfs);

    return res;
}

STATUS memfile_create(const char *name, u64 base_address, u64 len, u32 flags)
{
    if (strlen(name) + 1 >= MEMFS_MAX_FILENAME)
    {
        return FAILED;
    }

    memfile_t *memfile = kmalloc(sizeof(memfile_t));

    if (memfile == NULL)
    {
        return FAILED;
    }

    strcpy(memfile->name, name);

    memfile->base_address = base_address;
    memfile->size = len;
    memfile->open_count = 0;
    memfile->flags = flags;

    if (hashmap_put(memfiles_map, memfile->name, memfile) != MAP_OK)
    {
        kfree(memfile);
        return FAILED;
    }

    return SUCCESS;
}

STATUS memfile_destroy(const char *name)
{
    memfile_t *memfile = NULL;

    if (hashmap_get(memfiles_map, name, (void **)&memfile) == MAP_MISSING)
    {
        return FAILED;
    }

    if (memfile->open_count != 0)
    {
        return FAILED;
    }

    hashmap_remove(memfiles_map, name);

    kfree(memfile);

    return SUCCESS;
}

static file_t *__open(const char *name, u32 flags)
{
    (void)flags; // TODO: check perms
    memfile_t *memfile = NULL;

    if (hashmap_get(memfiles_map, name, (void **)&memfile) == MAP_MISSING)
    {
        return NULL;
    }

    file_t *file = kmalloc(sizeof(file_t));

    if (file == NULL)
    {
        return NULL;
    }

    file->data = memfile;

    memfile->open_count += 1;

    return file;
}

static s32 __lseek(file_t *file, u64 offset, u32 whence)
{
    memfile_t *memfile = file->data;

    switch (whence)
    {
    case FS_SEEK_SET:
        if (offset > memfile->size)
        {
            return -1;
        }

        file->cursor_position = offset;
        break;
    case FS_SEEK_CUR:
        if (file->cursor_position + offset > memfile->size)
        {
            return -1;
        }

        file->cursor_position += offset;
        break;
    case FS_SEEK_END:
        if (memfile->size + offset > memfile->size)
        {
            return -1;
        }

        file->cursor_position = memfile->size + offset;
        break;
    default:
        return -1;
    }

    return file->cursor_position;
}

static void __destroy_file(file_t *file)
{
    memfile_t *data = file->data;

    if (data != NULL)
    {
        data->open_count -= 1;
    }

    kfree(file);
}

static s32 __write(file_t *file, u8 *buffer, u32 size)
{
    memfile_t *data = file->data;

    /** Set the buffer at the current cursor position **/
    u8 *mem_buff = ((u8 *)data->base_address) + file->cursor_position;

    for (u32 i = 0; i < size; ++i)
    {
        if (file->cursor_position + i >= data->size)
        {
            file->cursor_position += i;
            return i;
        }

        mem_buff[i] = buffer[i];
    }

    file->cursor_position += size;

    return size;
}

static s32 __read(file_t *file, u8 *buffer, u32 size)
{
    memfile_t *data = file->data;

    /** Set the buffer at the current cursor position **/
    u8 *mem_buff = ((u8 *)data->base_address) + file->cursor_position;

    for (u32 i = 0; i < size; ++i)
    {
        if (file->cursor_position + i >= data->size)
        {
            file->cursor_position += i;
            return i;
        }

        buffer[i] = mem_buff[i];
    }

    file->cursor_position += size;

    return size;
}