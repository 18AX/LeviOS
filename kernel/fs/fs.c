#include "levi/fs/fs.h"

#include "levi/memory/memory.h"
#include "levi/stivale2.h"
#include "levi/utils/string.h"
#include "levi/utils/stringmap.h"

static map_t fs_map = NULL;

STATUS init_fs(void)
{
    fs_map = hashmap_new();

    if (fs_map == NULL)
    {
        return FAILED;
    }

    return SUCCESS;
}

STATUS register_fs(struct vfs *vfs)
{
    struct vfs *res = NULL;
    if (hashmap_get(fs_map, vfs->name, (void **)&res) == MAP_OK)
    {
        return FAILED;
    }

    if (hashmap_put(fs_map, vfs->name, vfs) == MAP_OMEM)
    {
        return FAILED;
    }

    return SUCCESS;
}

STATUS unregister_fs(struct vfs *vfs)
{
    if (hashmap_remove(fs_map, vfs->name) == MAP_MISSING)
    {
        return FAILED;
    }

    return SUCCESS;
}

file_t *open_file(const char *path, u32 flags)
{
    char *elements[2];

    if (split(path, ':', elements) == FAILED)
    {
        return NULL;
    }

    char *key = elements[0];
    char *value = elements[1];

    struct vfs *vfs = NULL;

    if (hashmap_get(fs_map, key, (void **)&vfs) == MAP_MISSING)
    {
        kfree(key);
        kfree(value);
        return NULL;
    }

    if (vfs->operation == NULL || vfs->operation->open == NULL)
    {
        kfree(key);
        kfree(value);
        return NULL;
    }

    file_t *file = vfs->operation->open(value, flags);

    if (file == NULL)
    {
        return NULL;
    }

    file->vfs = vfs;
    file->flags = flags;
    file->cursor_position = 0;

    kfree(key);
    kfree(value);

    if (file == NULL)
    {
        return NULL;
    }

    return file;
}

void destroy_file(file_t *file)
{
    if (file == NULL || file->vfs == NULL || file->vfs->operation == NULL)
    {
        return;
    }

    file->vfs->operation->destroy_file(file);
}