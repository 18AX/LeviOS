#ifndef MEM_POOL_HEADER
#define MEM_POOL_HEADER

#include <levi/types.h>

typedef struct
{
    u8 *mem_ptr;
    u64 current_size;
    u64 element_size;
    u8 *next_free_blk;
    u64 nb_total_blk;
    u64 nb_free_blk;
    u64 nb_initialized_blk;
} mem_pool_t;

/**
 * @brief Allocate a new pool of size nb_elements * elements_size
 *
 * @param nb_elements
 * @param element_size
 * @return mem_pool_t*
 */
mem_pool_t *mem_pool_new(u64 nb_elements, u64 element_size);

/**
 * @brief Allocate a block of the element size
 *
 * @param pool
 */
void *mem_pool_alloc(mem_pool_t *pool);

/**
 * @brief expand the memory pool with n elements.
 *
 * @param nb_element_to_add
 */
STATUS mem_pool_expand(mem_pool_t *pool, u64 nb_element_to_add);

/**
 * @brief free a block of the element size
 *
 * @param pool
 * @param ptr
 * @return void*
 */
void mem_pool_free(mem_pool_t *pool, void *ptr);

/**
 * @brief Destroy a mempool
 *
 * @param pool
 */
void mem_pool_destroy(mem_pool_t *pool);

#endif