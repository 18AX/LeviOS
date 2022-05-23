#ifndef LIST_HEADER
#define LIST_HEADER

#include <levi/types.h>

struct list_element
{
    void *data;
    struct list_element *prev;
    struct list_element *next;
};

typedef struct list
{
    struct list_element *head;
    struct list_element *tail;
    u64 size;
} list_t;

struct list *list_create();

void list_add(struct list *list, void *elt);

void list_pop_front(struct list *list, void (*free_func)(void *));

void list_for_each(struct list *list, u32 (*func)(u32, void *));

void list_for_each_rev(struct list *list, u32 (*func)(u32, void *));

void list_destroy(struct list *list, void (*free_func)(void *));

#endif