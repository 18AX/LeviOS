#ifndef VMM_HEADER
#define VMM_HEADER

#include "levi/types.h"

#define VM_PRESENT 0x1
#define VM_READ_WRITE 0x2
#define VM_USER 0x4
#define VM_EXECUTE_DISABLE (1 << 11)

#define MAP_FAILED 0
#define MAP_SUCCESS 1
#define MAP_PRESENT 2

#define MAP_STATUS u32

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

MAP_STATUS vmmap(vas_t *vas, u64 physical, u64 virt, u64 flags);

MAP_STATUS vmmap_range(vas_t *vas, u64 physical, u64 virt, u64 size, u64 flags);

MAP_STATUS vmunmap(vas_t *vas, u64 virts);

MAP_STATUS vmunmap_range(vas_t *vas, u64 virt, u64 size);

MAP_STATUS vma_to_phys(vas_t *vas, u64 virt, u64 *phys);

MAP_STATUS vascpy(vas_t *dst, vas_t *src);

/**
 * @brief Free a virtual address space. The page contained in the last layer of
 * the pml are freed too.
 *
 * @param vas
 */
void destroy_vas(vas_t *vas);

#endif