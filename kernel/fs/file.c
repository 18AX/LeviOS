#include <levi/fs/file.h>
#include <levi/fs/fs.h>

s32 open(const char *pathname, u32 flags, proc_t *process)
{
    file_t *file = open_file(pathname, flags);

    if (file == NULL)
    {
        return -1;
    }

    for (u32 i = 0; i < FD_TABLE_LEN; ++i)
    {
        if (process->fds[i] == NULL)
        {
            process->fds[i] = file;
            file->open_count = 1;

            return i;
        }
    }

    destroy_file(file);

    return -1;
}

STATUS close(s32 fd, proc_t *process)
{
    if (fd < 0 || fd >= FD_TABLE_LEN || process->fds[fd] == NULL)
    {
        return FAILED;
    }

    file_t *f = process->fds[fd];

    f->open_count -= 1;

    if (f->open_count == 0)
    {
        destroy_file(f);
    }

    process->fds[fd] = NULL;

    return SUCCESS;
}

s32 write(s32 fd, void *buffer, u32 size, proc_t *process)
{
    if (fd < 0 || fd >= FD_TABLE_LEN || process->fds[fd] == NULL)
    {
        return -1;
    }

    file_t *f = process->fds[fd];

    if ((f->flags & FS_WRITE) == 0)
    {
        return -1;
    }

    if (f->vfs == NULL || f->vfs->operation == NULL
        || f->vfs->operation->write == NULL)
    {
        return -1;
    }

    return f->vfs->operation->write(f, (u8 *)buffer, size);
}

s32 read(s32 fd, void *buffer, u32 size, proc_t *process)
{
    if (fd < 0 || fd >= FD_TABLE_LEN || process->fds[fd] == NULL)
    {
        return -1;
    }

    file_t *f = process->fds[fd];

    if (f->vfs == NULL || f->vfs->operation == NULL
        || f->vfs->operation->read == NULL)
    {
        return -1;
    }

    return f->vfs->operation->read(f, (u8 *)buffer, size);
}

s32 kopen(const char *pathname, u32 flags)
{
    proc_t *kern_proc = proc_get(0);

    return open(pathname, flags, kern_proc);
}

STATUS kclose(s32 fd)
{
    proc_t *kern_proc = proc_get(0);

    return close(fd, kern_proc);
}

s32 kwrite(u32 fd, void *buffer, u32 size)
{
    proc_t *kern_proc = proc_get(0);

    return write(fd, buffer, size, kern_proc);
}

s32 kread(u32 fd, void *buffer, u32 size)
{
    proc_t *kern_proc = proc_get(0);

    return read(fd, buffer, size, kern_proc);
}