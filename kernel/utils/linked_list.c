#include <levi/memory/memory.h>
#include <levi/utils/linked_list.h>

linked_list_t *linked_list_new(void (*free_func)(void *))
{
    linked_list_t *list = kmalloc(sizeof(linked_list_t));

    if (list == NULL)
    {
        return NULL;
    }

    list->free_func = free_func;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

u32 linked_list_add(linked_list_t *list, void *value)
{
    if (list == NULL)
    {
        return 0;
    }

    struct linked_list_elt *elt = kmalloc(sizeof(struct linked_list_elt));

    if (elt == NULL)
    {
        return 0;
    }

    elt->value = value;
    elt->prev = list->tail;
    elt->next = NULL;

    if (list->tail != NULL)
    {
        list->tail->next = elt;
        list->head->prev = elt;
    }
    else
    {
        list->head = elt;
    }

    list->tail = elt;
    ++list->size;

    return 1;
}

void *linked_list_get(linked_list_t *list, u32 index)
{
    if (list == NULL)
    {
        return NULL;
    }

    struct linked_list_elt *elt = list->head;

    for (u32 i = 0; elt != NULL; ++i)
    {
        if (i == index)
        {
            return elt->value;
        }

        elt = elt->next;
    }

    return NULL;
}

void *linked_list_pop_tail(linked_list_t *list)
{
    if (list == NULL || list->tail == NULL)
    {
        return NULL;
    }

    struct linked_list_elt *tail = list->tail;

    void *value = tail->value;

    if (tail == list->head)
    {
        list->tail = NULL;
        list->head = NULL;
        list->size = 0;
    }
    else
    {
        tail->prev->next = NULL;
        --list->size;
    }

    list->tail = tail->prev;

    kfree(tail);

    return value;
}

void *linked_list_pop_front(linked_list_t *list)
{
    if (list == NULL || list->head == NULL)
    {
        return NULL;
    }

    struct linked_list_elt *head = list->head;

    void *value = head->value;

    if (head == list->tail)
    {
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
    }
    else
    {
        head->next->prev = NULL;
        --list->size;
    }

    list->head = head->next;

    kfree(head);

    return value;
}

static void remove_elt(linked_list_t *list, struct linked_list_elt *elt)
{
    if (elt->prev != NULL)
    {
        elt->prev->next = elt->next;
    }

    if (elt->next != NULL)
    {
        elt->next->prev = elt->prev;
    }

    if (list->head == elt)
    {
        list->head = elt->next;
    }

    if (list->tail == elt)
    {
        list->tail = elt->prev;
    }

    if (list->free_func != NULL)
    {
        list->free_func(elt->value);
    }

    kfree(elt);

    --list->size;
}

u32 linked_list_remove_at(linked_list_t *list, u32 index)
{
    if (list == NULL)
    {
        return 0;
    }

    struct linked_list_elt *elt = list->head;

    for (u32 i = 0; elt != NULL; ++i)
    {
        if (i == index)
        {
            remove_elt(list, elt);
            return 1;
        }

        elt = elt->next;
    }

    return 0;
}

u32 linked_list_remove_eq(linked_list_t *list, u32 (*eq_func)(void *))
{
    if (list == NULL || eq_func == NULL)
    {
        return 0;
    }

    u32 removed = 0;

    struct linked_list_elt *current = list->head;

    while (current != NULL)
    {
        struct linked_list_elt *tmp = current->next;

        if (eq_func(current->value) == 1)
        {
            remove_elt(list, current);
            ++removed;
        }

        current = tmp;
    }

    return removed;
}

u32 linked_list_remove_ptr(linked_list_t *list, void *ptr)
{
    if (list == NULL)
    {
        return 0;
    }

    u32 removed = 0;

    struct linked_list_elt *current = list->head;

    while (current != NULL)
    {
        struct linked_list_elt *tmp = current->next;

        if (current->value == ptr)
        {
            remove_elt(list, current);
        }

        current = tmp;
    }

    return removed;
}

void linked_list_for_each(linked_list_t *list, void (*func)(u32, void *value))
{
    if (list == NULL || func == NULL)
    {
        return;
    }

    struct linked_list_elt *current = list->head;

    for (u32 i = 0; current != NULL; ++i)
    {
        func(i, current->value);
        current = current->next;
    }
}

void linked_list_for_each_rev(linked_list_t *list,
                              void (*func)(u32, void *value))
{
    if (list == NULL || func == NULL)
    {
        return;
    }

    struct linked_list_elt *current = list->tail;

    for (u32 i = list->size - 1; current != NULL; --i)
    {
        func(i, current->value);
        current = current->prev;
    }
}

u64 linked_list_size(linked_list_t *list)
{
    if (list == NULL)
    {
        return 0;
    }

    return list->size;
}

void linked_list_free(linked_list_t *list)
{
    if (list == NULL)
    {
        return;
    }

    // Free all the list elements
    struct linked_list_elt *current = list->head;

    while (current != NULL)
    {
        struct linked_list_elt *tmp = current->next;

        if (list->free_func != NULL)
        {
            list->free_func(current->value);
        }

        kfree(current);
        current = tmp;
    }
    kfree(list);
}