#include <levi/memory/memory.h>
#include <levi/utils/queue.h>
#include <levi/utils/string.h>

queue_t *queue_init(u64 size)
{
    queue_t *queue = kmalloc(sizeof(queue_t));

    if (queue == NULL)
    {
        return NULL;
    }

    memset(queue, 0x0, sizeof(queue_t));

    queue->max_size = size;
    queue->data = kmalloc(sizeof(u64) * size);

    if (queue->data == NULL)
    {
        kfree(queue);
        return NULL;
    }

    return queue;
}

void queue_add(queue_t *queue, u64 data)
{
    queue->head = (queue->head + 1) % queue->max_size;

    queue->data[queue->head] = data;

    if (queue->size != queue->max_size)
    {
        queue->size++;
    }
}

STATUS queue_pop(queue_t *queue, u64 *value)
{
    if (queue->size == 0)
    {
        return FAILED;
    }

    *value = queue->data[queue->tail];

    queue->tail++;
    queue->size--;

    return SUCCESS;
}

u64 queue_size(queue_t *queue)
{
    return queue->size;
}

void queue_free(queue_t *queue)
{
    if (queue == NULL)
    {
        return;
    }

    kfree(queue->data);
    kfree(queue);
}
