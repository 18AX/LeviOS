#ifndef VMM_HEADER
#define VMM_HEADER

#include "levi/types.h"

#define VM_PRESENT 0x1
#define VM_READ_WRITE 0x2
#define VM_USER 0x4

/**
 * Those functions need to be implemented by the arch
 */

typedef struct vas_t
{
    void *data;
} vas_t;

STATUS fill_empty_vas(vas_t *vas);

STATUS switch_vas(vas_t *vas);

vas_t get_vas(void);

STATUS vmmap(vas_t *vas, u64 physical, u64 virt, u64 flags);

STATUS vmmap_range(vas_t *vas, u64 physical, u64 virt, u64 size, u64 flags);

STATUS vmunmap(vas_t *vas, u64 virts);

#endif