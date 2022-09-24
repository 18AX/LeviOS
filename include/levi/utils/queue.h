#ifndef QUEUE_HEADER
#define QUEUE_HEADER

#include <levi/types.h>

typedef struct
{
    u64 size;
    u64 max_size;
    u64 head;
    u64 tail;
    u64 *data;
} queue_t;

queue_t *queue_init(u64 size);

void queue_add(queue_t *queue, u64 data);

STATUS queue_pop(queue_t *queue, u64 *value);

u64 queue_size(queue_t *queue);

void queue_free(queue_t *queue);

#endif