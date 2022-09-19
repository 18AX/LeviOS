#include <levi/drivers/fs/iso9660.h>
#include <levi/fs/file.h>
#include <levi/memory/memory.h>
#include <levi/utils/string.h>

static file_t *__open(vfs *vfs, const char *name, u32 flags);
static s64 __read(file_t *file, u8 *buffer, u64 size);
static void __destroy_file(file_t *file);
static s64 __lseek(file_t *file, u64 offset, u32 whence);

static struct vfs_operation iso_operation = {
    .mkdir = NULL,
    .destroy_file = __destroy_file,
    .fdfunc = NULL,
    .flush = NULL,
    .lseek = __lseek,
    .open = __open,
    .read = __read,
    .write = NULL,
    .rename = NULL,
    .rmdir = NULL,
};

/**
 * @brief Check if the fd corresponds to an iso file system
 *
 * @param fd
 * @return STATUS
 */
static STATUS check_iso(u32 fd)
{
    if (klseek(fd, ISO_BLK_OFFSET(ISO_PRIMARY_DESC_BLK), FS_SEEK_SET) == -1)
    {
        return FAILED;
    }

    char buffer[6];

    if (kread(fd, buffer, 6) == -1)
    {
        return FAILED;
    }

    if (strncmp(buffer + 1, ISO_SIG, ISO_SIG_LEN) == 0)
    {
        return SUCCESS;
    }

    return FAILED;
}

STATUS iso_mount(u32 fd, const char *name)
{
    if (check_iso(fd) == FAILED)
    {
        return FAILED;
    }

    s32 newfd = kdup(fd);

    if (newfd == -1)
    {
        return FAILED;
    }

    vfs *vfs = kmalloc(sizeof(vfs));

    if (vfs == NULL)
    {
        return FAILED;
    }

    vfs->data = (void *)((u64)newfd);
    vfs->flags = 0x0;
    vfs->name = strdup(name);
    vfs->operation = &iso_operation;

    if (vfs->name == NULL)
    {
        kfree(vfs);
        return FAILED;
    }

    return register_fs(vfs);
}

STATUS iso_unmount(vfs *iso_vfs)
{
    u64 fd = (u64)iso_vfs->data;
    kclose(fd);

    STATUS s = unregister_fs(iso_vfs);

    kfree(iso_vfs->name);
    kfree(iso_vfs);

    return s;
}

static s64 find_next(char *str, char c)
{
    for (u64 i = 0; str[i] != '\0'; ++i)
    {
        if (str[i] == c)
        {
            return i;
        }
    }

    return -1;
}

/**
 * @brief compare filename to an iso_directory identifier. We need a custom
 * function because the filename ends by a ;
 *
 * @param filename
 * @param identifier
 * @param identifier_len
 * @return u64
 */
static u64 iso_filename_eq(const char *filename, const char *identifier,
                           u64 identifier_len)
{
    for (u64 i = 0;
         i < identifier_len && identifier[i] != ';' && filename[i] != '\0'; ++i)
    {
        if (filename[i] != identifier[i])
        {
            return 0;
        }
    }

    return 1;
}

static struct iso_directory *find_file_in_dir(u32 fd, char *filename, u32 blk)
{
    /** For now we only support directory on one block **/
    struct iso_directory *dir = kmalloc(ISO_BLK_SIZE);

    if (dir == NULL)
    {
        return NULL;
    }

    klseek(fd, ISO_BLK_OFFSET(blk), FS_SEEK_SET);

    if (kread(fd, dir, ISO_BLK_SIZE) < 0)
    {
        kfree(dir);
        return NULL;
    }

    u8 *current = (u8 *)dir;
    u8 *limit = current + ISO_BLK_SIZE;

    while (current < limit)
    {
        struct iso_directory *tmp = (struct iso_directory *)current;

        if (tmp->record_len == 0)
        {
            break;
        }

        if (!ISO_IS_DIR(tmp->file_flags)
            && iso_filename_eq(filename, tmp->identifier, tmp->identifier_len)
                == 1)
        {
            struct iso_directory *copy = kmalloc(tmp->record_len);

            if (copy == NULL)
            {
                break;
            }

            memcpy(copy, tmp, tmp->record_len);
            kfree(dir);

            return copy;
        }

        current += tmp->record_len;
    }

    kfree(dir);
    return NULL;
}

/**
 * @brief Find the corresponding pathtable for a directory name
 *
 * @param pathtable
 * @param parent
 * @param dir_name
 * @param limit_address
 * @param saved_id
 * @return struct iso_pathtable*
 */
static struct iso_pathtable *pt_find_next(struct iso_pathtable *pathtable,
                                          u16 parent, const char *dir_name,
                                          u8 *limit_address, u16 *saved_id)
{
    u8 *current = (u8 *)pathtable;

    u64 len = strlen(dir_name);

    while (current < limit_address)
    {
        struct iso_pathtable *tmp_pt = (struct iso_pathtable *)current;

        if (tmp_pt->parent_id == parent && len == tmp_pt->identifier_len
            && strncmp(dir_name, tmp_pt->identifier, tmp_pt->identifier_len)
                == 0)
        {
            return tmp_pt;
        }

        current += sizeof(struct iso_pathtable) + tmp_pt->identifier_len;

        if (tmp_pt->identifier_len % 2 != 0)
        {
            current += 1;
        }

        *saved_id += 1;
    }

    return NULL;
}

/**
 * @brief Find a file using the path table
 *
 * @param pathtable
 * @param name
 * @return struct iso_directory*
 */
static struct iso_directory *find_file(u32 fd, struct iso_pathtable *pathtable,
                                       u32 pathtable_size, const char *name)
{
    char *filename = strdup(name);

    if (filename == NULL)
    {
        return NULL;
    }

    strtoupper(filename);

    char *filename_ptr = filename;

    if (*filename_ptr == '/')
    {
        ++filename_ptr;
    }

    // root directory
    u16 parent_id = pathtable->parent_id;
    u16 saved_id = parent_id;

    struct iso_pathtable *current_ptr = pathtable;
    u8 *limit = ((u8 *)current_ptr) + pathtable_size;

    while (*filename_ptr != '\0')
    {
        /** Find the position of the next '/' to extract directory name **/
        s64 dir_name_end = find_next(filename_ptr, '/');

        /** No '/', that means we are on the filename **/
        if (dir_name_end == -1)
        {
            struct iso_directory *r =
                find_file_in_dir(fd, filename_ptr, current_ptr->location_blk);

            kfree(filename);

            return r;
        }

        filename_ptr[dir_name_end] = '\0';

        /** Try to find the pathtable entry for this directory name **/
        struct iso_pathtable *dir = pt_find_next(
            current_ptr, parent_id, filename_ptr, limit, &saved_id);

        if (dir == NULL)
        {
            kfree(filename);
            return NULL;
        }

        filename_ptr = &filename_ptr[dir_name_end + 1];

        current_ptr = dir;
        parent_id = saved_id;
    }

    kfree(filename);

    return NULL;
}

static file_t *__open(vfs *vfs, const char *name, u32 flags)
{
    /**
     * @brief To find a file we need to:
     * - read primary descriptor to get pathtable location
     * - read pathtable
     * - find directory in pathtable
     */
    u64 fd = (u64)vfs->data;

    if (klseek(fd, ISO_BLK_OFFSET(ISO_PRIMARY_DESC_BLK), FS_SEEK_SET) == -1)
    {
        return NULL;
    }

    /** Read the primary descriptor to get path table location and size **/
    struct iso_primary_desc primary_desc = { 0 };

    if (kread(fd, &primary_desc, sizeof(struct iso_primary_desc)) < 0)
    {
        return NULL;
    }

    if (klseek(fd, ISO_BLK_OFFSET(primary_desc.pathtable_blk_lsb), FS_SEEK_SET)
        == -1)
    {
        return NULL;
    }

    /** allocate and read the pathtable **/
    struct iso_pathtable *pathtable = kmalloc(primary_desc.pathtable_size_lsb);

    if (pathtable == NULL)
    {
        return NULL;
    }

    if (kread(fd, pathtable, primary_desc.pathtable_size_lsb) < 0)
    {
        kfree(pathtable);
        return NULL;
    }

    /** The iso_directory struct is already allocated **/
    struct iso_directory *iso_file =
        find_file(fd, pathtable, primary_desc.pathtable_size_lsb, name);

    if (iso_file == NULL)
    {
        kfree(pathtable);
        return NULL;
    }

    file_t *file = kmalloc(sizeof(file_t));

    if (file == NULL)
    {
        kfree(pathtable);
        return NULL;
    }

    file->cursor_position = 0;
    file->flags = flags;
    file->data = iso_file;

    kfree(pathtable);

    return file;
}

static s64 __read(file_t *file, u8 *buffer, u64 size)
{
    u64 fd = (u64)file->vfs->data;

    struct iso_directory *iso_file = (struct iso_directory *)file->data;

    if (iso_file == NULL)
    {
        return -1;
    }

    u64 offset =
        ISO_BLK_OFFSET(iso_file->location_blk_lsb) + file->cursor_position;

    if (klseek(fd, offset, FS_SEEK_SET) < 0)
    {
        return -1;
    }

    u64 to_read = size;

    if (file->cursor_position + to_read > iso_file->data_len_lsb)
    {
        to_read = iso_file->data_len_lsb - file->cursor_position;
    }

    u64 read = kread(fd, buffer, to_read);

    file->cursor_position += read;

    return read;
}

static s64 __lseek(file_t *file, u64 offset, u32 whence)
{
    struct iso_directory *iso_file = (struct iso_directory *)file->data;

    switch (whence)
    {
    case FS_SEEK_CUR:
        if (file->cursor_position + offset > iso_file->data_len_lsb)
        {
            return -1;
        }

        file->cursor_position += offset;
        break;
    case FS_SEEK_SET:
        if (offset > iso_file->data_len_lsb)
        {
            return -1;
        }

        file->cursor_position = offset;
        break;
    case FS_SEEK_END:
        if (file->cursor_position + offset > iso_file->data_len_lsb)
        {
            return -1;
        }

        file->cursor_position = iso_file->data_len_lsb + offset;
        break;
    default:
        return -1;
        break;
    }

    return file->cursor_position;
}

static void __destroy_file(file_t *file)
{
    if (file != NULL)
    {
        /** We free the iso_directory struct **/
        if (file->data != NULL)
        {
            kfree(file->data);
        }

        kfree(file);
    }
}