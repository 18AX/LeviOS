#ifndef FILE_HEADER
#define FILE_HEADER

#include <levi/proc/process.h>
#include <levi/types.h>

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

s32 open(const char *pathname, u32 flags, proc_t *process);

STATUS close(s32 fd, proc_t *prcess);

s64 write(s32 fd, void *buffer, u64 size, proc_t *process);

s64 read(s32 fd, void *buffer, u64 size, proc_t *process);

s32 flush(s32 fd, proc_t *process);

s64 lseek(s32 fd, u64 offset, u32 whence, proc_t *process);

s32 dup2(s32 oldfd, s32 newfd, proc_t *process);

s32 dup(s32 oldfd, proc_t *process);

s32 kopen(const char *pathname, u32 flags);

s64 fdfunc(s32 fd, u64 func_id, u64 args0, u64 args1, proc_t *process);

STATUS kclose(s32 fd);

s64 kwrite(u32 fd, void *buffer, u64 size);

s64 kread(u32 fd, void *buffer, u64 size);

s32 kflush(u32 fd);

s64 klseek(s32 fd, u64 offset, u32 whence);

s32 kdup2(s32 oldfd, s32 newfd);

s32 kdup(s32 oldfd);

s64 kfdfunc(s32 fd, u64 func_id, u64 args0, u64 args1);

#endif