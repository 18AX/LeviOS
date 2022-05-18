#ifndef EXEC_HEADER
#define EXEC_HEADER

#include <levi/types.h>

/**
 * @brief create a process from a file descriptor to the file.
 *
 * @param address the address where the memory is mapped
 * @param size size of the
 * @return STATUS
 */
STATUS exec_from_fd(s32 fd, u32 flags);

#endif