#include <string.h>
#include <syscall.h>

#define SYSCALL_FAILED 0xFFFFFFFFFFFFFFFF

static u64 sys_exec(u64 sys_id, u64 arg0, u64 arg1, u64 arg2, u64 arg3)
{
    u64 ret = 0;

    asm volatile("movq %[a0],%%rdi\n"
                 "movq %[a1],%%rsi\n"
                 "movq %[a2],%%rdx\n"
                 "movq %[a3],%%r10\n"
                 "movq %[id],%%rax\n"
                 "int $0x80\n"
                 "movq %%rax,%[retv]\n"
                 : [retv] "=r"(ret)
                 : [a0] "r"(arg0), [a1] "r"(arg1), [a2] "r"(arg2),
                   [a3] "r"(arg3), [id] "r"(sys_id)
                 : "rdi", "rsi", "rdx", "r10", "rax");

    return ret;
}

s32 open(const char *pathname, u32 flags)
{
    u64 len = strlen(pathname);
    u64 ret = sys_exec(SYSCALL_OPEN, (u64)pathname, len, (u64)flags, 0);

    if (ret == SYSCALL_FAILED)
    {
        return -1;
    }

    return ret;
}

s64 close(s32 fd)
{
    u64 ret = sys_exec(SYSCALL_CLOSE, (u64)fd, 0, 0, 0);

    if (ret == SYSCALL_FAILED)
    {
        return -1;
    }

    return ret;
}

s64 write(s32 fd, const void *buffer, u64 size)
{
    u64 ret = sys_exec(SYSCALL_WRITE, (u64)fd, (u64)buffer, size, 0);

    if (ret == SYSCALL_FAILED)
    {
        return -1;
    }

    return ret;
}

s64 read(s32 fd, void *buffer, u64 size)
{
    u64 ret = sys_exec(SYSCALL_WRITE, (u64)fd, (u64)buffer, size, 0);

    if (ret == SYSCALL_FAILED)
    {
        return -1;
    }

    return ret;
}

s32 flush(s32 fd)
{
    u64 ret = sys_exec(SYSCALL_FLUSH, (u64)fd, 0, 0, 0);

    if (ret == SYSCALL_FAILED)
    {
        return -1;
    }

    return ret;
}

s64 lseek(s32 fd, u64 offset, u32 whence)
{
    u64 ret = sys_exec(SYSCALL_LSEEK, (u64)fd, offset, (u64)whence, 0);

    if (ret == SYSCALL_FAILED)
    {
        return -1;
    }

    return ret;
}

s32 dup2(s32 oldfd, s32 newfd)
{
    u64 ret = sys_exec(SYSCALL_DUP2, (u64)oldfd, (u64)newfd, 0, 0);

    if (ret == SYSCALL_FAILED)
    {
        return -1;
    }

    return ret;
}

s64 exec(const char *path, const char *name, const char *argv[],
         const char *envp[])
{
    u64 ret =
        sys_exec(SYSCALL_EXEC, (u64)path, (u64)name, (u64)argv, (u64)envp);

    if (ret == SYSCALL_FAILED)
    {
        return -1;
    }

    return ret;
}

s64 exit(s64 exit_status)
{
    u64 ret = sys_exec(SYSCALL_EXIT, exit_status, 0, 0, 0);

    if (ret == SYSCALL_FAILED)
    {
        return -1;
    }

    return ret;
}

void *mmap(void *address, u64 nb_page, u64 flags)
{
    u64 ret = sys_exec(SYSCALL_MMAP, (u64)address, nb_page, flags, 0);

    if (ret == SYSCALL_FAILED)
    {
        return NULL;
    }

    return (void *)ret;
}

void gettime(struct time *time)
{
    sys_exec(SYSCALL_GETTIME, (u64)time, 0x0, 0X0, 0x0);
}

u64 getticks()
{
    return sys_exec(SYSCALL_GETTICKS, 0x0, 0x0, 0x0, 0x0);
}