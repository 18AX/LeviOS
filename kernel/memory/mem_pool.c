#include <levi/memory/mem_pool.h>
#include <levi/memory/memory.h>

mem_pool_t *mem_pool_new(u64 nb_elements, u64 element_size)
{
    u64 total_size = nb_elements * element_size;
    void *mem_ptr = kmalloc(total_size);

    if (mem_ptr == NULL)
    {
        return NULL;
    }

    mem_pool_t *pool = kmalloc(sizeof(mem_pool_t));

    if (pool == NULL)
    {
        kfree(mem_ptr);
        return NULL;
    }

    pool->mem_ptr = mem_ptr;
    pool->current_size = total_size;
    pool->element_size = element_size;
    pool->next_free_blk = mem_ptr;
    pool->nb_free_blk = nb_elements;
    pool->nb_total_blk = nb_elements;
    pool->nb_initialized_blk = 0;

    return pool;
}

static void *offset_by_index(mem_pool_t *pool, u64 index)
{
    return (void *)(pool->mem_ptr + (pool->element_size * index));
}

void *mem_pool_alloc(mem_pool_t *pool)
{
    if (pool->nb_initialized_blk < pool->nb_total_blk)
    {
        u64 *idx = offset_by_index(pool, pool->nb_initialized_blk);

        *idx = pool->nb_initialized_blk + 1;
        pool->nb_initialized_blk++;
    }

    void *ret = NULL;

    if (pool->nb_free_blk > 0)
    {
        ret = pool->next_free_blk;
        pool->nb_free_blk--;

        if (pool->nb_free_blk != 0)
        {
            pool->next_free_blk =
                offset_by_index(pool, *((u64 *)pool->next_free_blk));
        }
        else
        {
            pool->next_free_blk = NULL;
        }
    }

    return ret;
}

STATUS mem_pool_expand(mem_pool_t *pool, u64 nb_element_to_add)
{
    if (pool == NULL)
    {
        return FAILED;
    }

    u64 new_size = pool->current_size + nb_element_to_add * pool->element_size;
    void *ptr = krealloc(pool->mem_ptr, new_size);

    if (ptr == NULL)
    {
        return FAILED;
    }

    pool->mem_ptr = ptr;
    pool->element_size = new_size;
    pool->nb_free_blk += nb_element_to_add;
    pool->nb_total_blk += nb_element_to_add;

    return SUCCESS;
}

void mem_pool_free(mem_pool_t *pool, void *ptr)
{
    if (pool->next_free_blk != NULL)
    {
        *((u64 *)ptr) = *((u64 *)pool->next_free_blk);
        pool->next_free_blk = ptr;
    }
    else
    {
        *((u64 *)ptr) = pool->nb_initialized_blk;
        pool->next_free_blk = ptr;
    }

    pool->nb_free_blk++;
}

void mem_pool_destroy(mem_pool_t *pool)
{
    if (pool != NULL)
    {
        kfree(pool->mem_ptr);
        kfree(pool);
    }
}
