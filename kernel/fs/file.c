#include <levi/fs/file.h>
#include <levi/fs/fs.h>

s32 open(const char *pathname, u32 flags, proc_t *process)
{
    file_t *file = open_file(pathname, flags);

    if (file == NULL)
    {
        return -1;
    }

    /** The first 3 fd are reserved for stdout, stdint and stderr **/
    for (u32 i = 3; i < FD_TABLE_LEN; ++i)
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

    if (f->vfs == NULL || f->vfs->operation == NULL
        || f->vfs->operation->destroy_file == NULL)
    {
        return FAILED;
    }

    f->open_count -= 1;

    if (f->open_count == 0)
    {
        destroy_file(f);
    }

    process->fds[fd] = NULL;

    return SUCCESS;
}

s64 write(s32 fd, void *buffer, u64 size, proc_t *process)
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

s64 read(s32 fd, void *buffer, u64 size, proc_t *process)
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

s32 flush(s32 fd, proc_t *process)
{
    if (fd < 0 || fd >= FD_TABLE_LEN || process->fds[fd] == NULL)
    {
        return -1;
    }

    file_t *f = process->fds[fd];

    if (f->vfs == NULL || f->vfs->operation == NULL
        || f->vfs->operation->flush == NULL)
    {
        return -1;
    }

    return f->vfs->operation->flush(f);
}

s64 lseek(s32 fd, u64 offset, u32 whence, proc_t *process)
{
    if (fd < 0 || fd >= FD_TABLE_LEN || process->fds[fd] == NULL)
    {
        return -1;
    }

    file_t *f = process->fds[fd];

    if (f->vfs == NULL || f->vfs->operation == NULL
        || f->vfs->operation->lseek == NULL)
    {
        return -1;
    }

    return f->vfs->operation->lseek(f, offset, whence);
}

s32 dup(s32 oldfd, proc_t *process)
{
    for (u32 i = 0; i < FD_TABLE_LEN; ++i)
    {
        if (process->fds[i] == NULL)
        {
            return dup2(oldfd, i, process);
        }
    }

    return -1;
}

s32 dup2(s32 oldfd, s32 newfd, proc_t *process)
{
    if (oldfd < 0 || oldfd >= FD_TABLE_LEN || newfd < 0 || newfd >= FD_TABLE_LEN
        || process->fds[oldfd] == NULL)
    {
        return -1;
    }

    if (process->fds[newfd] != NULL)
    {
        close(newfd, process);
    }

    file_t *file = process->fds[oldfd];

    file->open_count += 1;

    process->fds[newfd] = file;

    return newfd;
}

s64 fdfunc(s32 fd, u64 func_id, u64 args0, u64 args1, u64 args2,
           proc_t *process)
{
    if (fd < 0 || fd >= FD_TABLE_LEN || process->fds[fd] == NULL)
    {
        return -1;
    }

    file_t *f = process->fds[fd];

    if (f->vfs == NULL || f->vfs->operation == NULL
        || f->vfs->operation->lseek == NULL)
    {
        return -1;
    }

    return f->vfs->operation->fdfunc(f, func_id, args0, args1, args2);
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

s64 kwrite(u32 fd, void *buffer, u64 size)
{
    proc_t *kern_proc = proc_get(0);

    return write(fd, buffer, size, kern_proc);
}

s64 kread(u32 fd, void *buffer, u64 size)
{
    proc_t *kern_proc = proc_get(0);

    return read(fd, buffer, size, kern_proc);
}

s32 kflush(u32 fd)
{
    proc_t *kern_proc = proc_get(0);

    return flush(fd, kern_proc);
}

s64 klseek(s32 fd, u64 offset, u32 whence)
{
    proc_t *kern_proc = proc_get(0);

    return lseek(fd, offset, whence, kern_proc);
}

s32 kdup2(s32 oldfd, s32 newfd)
{
    proc_t *kern_proc = proc_get(0);

    return dup2(oldfd, newfd, kern_proc);
}

s32 kdup(s32 oldfd)
{
    proc_t *kern_proc = proc_get(0);

    return dup(oldfd, kern_proc);
}

s64 kfdfunc(s32 fd, u64 func_id, u64 args0, u64 args1, u64 args2)
{
    proc_t *kern_proc = proc_get(0);

    return fdfunc(fd, func_id, args0, args1, args2, kern_proc);
}