#ifndef FILE_HEADER
#define FILE_HEADER

#include "levi/proc/process.h"
#include "levi/types.h"

#define O_RDONLY 00
#define O_WRONLY 01
#define O_RDWR 02

s32 open(const char *pathname, u32 flags, proc_t *process);

STATUS close(s32 fd, proc_t *prcess);

s32 write(s32 fd, void *buffer, u32 size, proc_t *process);

s32 read(s32 fd, void *buffer, u32 size, proc_t *process);

s32 kopen(const char *pathname, u32 flags);

STATUS kclose(s32 fd);

s32 kwrite(u32 fd, void *buffer, u32 size);

s32 kread(u32 fd, void *buffer, u32 size);

#endif