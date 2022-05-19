#ifndef FILE_HEADER
#define FILE_HEADER

#include <levi/proc/process.h>
#include <levi/types.h>

s32 open(const char *pathname, u32 flags, proc_t *process);

STATUS close(s32 fd, proc_t *prcess);

s32 write(s32 fd, void *buffer, u32 size, proc_t *process);

s32 read(s32 fd, void *buffer, u32 size, proc_t *process);

s32 flush(s32 fd, proc_t *process);

s32 lseek(s32 fd, u64 offset, u32 whence, proc_t *process);

s32 kopen(const char *pathname, u32 flags);

STATUS kclose(s32 fd);

s32 kwrite(u32 fd, void *buffer, u32 size);

s32 kread(u32 fd, void *buffer, u32 size);

s32 kflush(u32 fd);

s32 klseek(s32 fd, u64 offset, u32 whence);

#endif