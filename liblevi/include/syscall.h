#ifndef SYSCALL_HEADER
#define SYSCALL_HEADER

#include <types.h>

#define SYSCALL_OPEN 0
#define SYSCALL_READ 1
#define SYSCALL_WRITE 2
#define SYSCALL_CLOSE 3
#define SYSCALL_LSEEK 4
#define SYSCALL_DUP2 5
#define SYSCALL_EXEC 5
#define SYSCALL_EXIT 7
#define SYSCALL_HEAP_ALLOC 8
#define SYSCALL_HEAP_FREE 9

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