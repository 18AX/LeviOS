#include <levi/drivers/framebuffer/framebuffer.h>
#include <levi/fs/fs.h>
#include <levi/memory/memory.h>
#include <levi/utils/string.h>

static file_t *__open(const char *name, u32 flags);
static void __destroy_file(file_t *file);
static s32 __write(file_t *file, u8 *buffer, u32 size);
static s32 __read(file_t *file, u8 *buffer, u32 size);
static s32 __flush(file_t *file);
static s32 __lseek(file_t *file, u64 offset, u32 whence);

static struct vfs_operation framebuffer_operation = { .mkdir = NULL,
                                                      .rmdir = NULL,
                                                      .rename = NULL,
                                                      .open = __open,
                                                      .write = __write,
                                                      .destroy_file =
                                                          __destroy_file,
                                                      .flush = __flush,
                                                      .read = __read,
                                                      .lseek = __lseek };

static vfs framebuffer_vfs = { .name = "framebuffer",
                               .flags = 0x0,
                               .operation = &framebuffer_operation };

static struct stivale2_struct_tag_framebuffer framebuffer_info = { 0 };

static u8 *video_buffer = NULL;
static u64 buffer_size;

STATUS framebuffer_init(struct stivale2_struct *boot_info)
{
    struct stivale2_struct_tag_framebuffer *framebuffer_tag =
        stivale2_get_tag(boot_info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);

    if (framebuffer_tag == NULL)
    {
        return FAILED;
    }

    memcpy(&framebuffer_info, framebuffer_tag,
           sizeof(struct stivale2_struct_tag_framebuffer));

    buffer_size = framebuffer_info.framebuffer_width
        * framebuffer_info.framebuffer_height
        * (framebuffer_info.framebuffer_bpp / 8);

    video_buffer = kmalloc(buffer_size);

    if (video_buffer == NULL)
    {
        return FAILED;
    }

    for (u64 i = 0; i < buffer_size; ++i)
    {
        video_buffer[i] = 0;
    }

    return register_fs(&framebuffer_vfs);
}

static file_t *__open(const char *name, u32 flags)
{
    (void)name; // We dont care about the name;
    (void)flags;

    file_t *file = kmalloc(sizeof(file_t));

    if (file == NULL)
    {
        return NULL;
    }

    file->data = NULL;

    return file;
}

static void __destroy_file(file_t *file)
{
    kfree(file);
}

static s32 __write(file_t *file, u8 *buffer, u32 size)
{
    u32 i = 0;

    for (; i < size && i + file->cursor_position < buffer_size; ++i)
    {
        video_buffer[i + file->cursor_position] = buffer[i];
    }

    file->cursor_position += i;

    return i;
}

static s32 __read(file_t *file, u8 *buffer, u32 size)
{
    u32 i = 0;

    for (; i < size && i + file->cursor_position < buffer_size; ++i)
    {
        buffer[i] = video_buffer[i + file->cursor_position];
    }

    file->cursor_position += i;

    return i;
}

static s32 __flush(file_t *file)
{
    (void)file;

    memcpy((void *)framebuffer_info.framebuffer_addr, video_buffer,
           buffer_size);

    return 0;
}

static s32 __lseek(file_t *file, u64 offset, u32 whence)
{
    switch (whence)
    {
    case FS_SEEK_SET:
        if (offset > buffer_size)
        {
            return -1;
        }

        file->cursor_position = offset;
        break;
    case FS_SEEK_CUR:
        if (file->cursor_position + offset > buffer_size)
        {
            return -1;
        }

        file->cursor_position += offset;
        break;
    case FS_SEEK_END:
        if (buffer_size + offset > buffer_size)
        {
            return -1;
        }

        file->cursor_position = buffer_size + offset;
        break;
    default:
        return -1;
    }

    return file->cursor_position;
}