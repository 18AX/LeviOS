#ifndef SYSCALL_HEADER
#define SYSCALL_HEADER

#include <types.h>

#define SYSCALL_OPEN 0
#define SYSCALL_READ 1
#define SYSCALL_WRITE 2
#define SYSCALL_CLOSE 3
#define SYSCALL_LSEEK 4
#define SYSCALL_FLUSH 5
#define SYSCALL_DUP2 6
#define SYSCALL_EXEC 7
#define SYSCALL_EXIT 8
#define SYSCALL_HEAP_ALLOC 9
#define SYSCALL_HEAP_FREE 10

#define FS_READ 0x0
#define FS_WRITE 0x1

#define FS_SEEK_SET 0
#define FS_SEEK_CUR 1
#define FS_SEEK_END 2

s32 open(const char *pathname, u32 flags);

s64 close(s32 fd);

s64 write(s32 fd, void *buffer, u64 size);

s64 read(s32 fd, void *buffer, u64 size);

s32 flush(s32 fd);

s64 lseek(s32 fd, u64 offset, u32 whence);

s32 dup2(s32 oldfd, s32 newfd);

s64 exec(const char *path, const char *name, const char *argv[],
         const char *envp[]);

s64 exit(s64 exit_status);

#endif