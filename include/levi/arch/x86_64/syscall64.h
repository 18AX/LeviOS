#ifndef SYSCALL64_HEADER
#define SYSCALL64_HEADER

#include <levi/types.h>

#define SYSCALL_GATE 0x80

/**
 * @brief Register a handler at the SYSCALL_GATE that call syscall function with
 * the right parameters.
 *
 * Conventions:
 * - rax store the syscall id and the return value
 * - rdi args0
 * - rsi args1
 * - rdx args2
 * - r10 args3
 *
 */
void syscall64_init(void);

#endif