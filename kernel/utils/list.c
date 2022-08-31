#include <levi/memory/memory.h>
#include <levi/utils/list.h>

struct list *list_create()
{
    struct list *n = kmalloc(sizeof(struct list));

    if (n == NULL)
    {
        return NULL;
    }

    n->head = NULL;
    n->tail = NULL;
    n->size = 0;

    return n;
}

void list_add(struct list *list, void *elt)
{
    if (list == NULL)
    {
        return;
    }

    struct list_element *n = kmalloc(sizeof(struct list_element));
    if (n == NULL)
    {
        return;
    }

    n->data = elt;
    n->next = NULL;

    if (list->head == NULL)
    {
        n->prev = NULL;
        list->head = n;
        list->tail = n;
        list->size = 0;
        return;
    }

    list->tail->next = n;
    n->prev = list->tail;
    list->tail = n;
    list->size += 1;
}

void *list_get(struct list *list, u64 pos)
{
    if (list == NULL || pos >= list->size)
    {
        return NULL;
    }

    struct list_element *current = list->head;

    u64 i = 0;
    while (current != NULL)
    {
        if (i == pos)
        {
            return current->data;
        }

        ++i;
        current = current->next;
    }

    return NULL;
}

void list_pop_front(struct list *list, void (*free_func)(void *))
{
    if (list == NULL || list->head == NULL)
    {
        return;
    }

    struct list_element *next_elt = list->head->next;

    free_func(list->head->data);
    kfree(list->head);

    if (next_elt != NULL)
    {
        next_elt->prev = NULL;
        list->head = next_elt;
    }

    list->size -= 1;
}

void list_for_each(struct list *list, u32 (*func)(u32, void *))
{
    if (list == NULL)
    {
        return;
    }

    struct list_element *current = list->head;

    u32 index = 0;

    while (current != NULL)
    {
        if (func(index, current->data) == 0)
        {
            break;
        }

        current = current->next;
        ++index;
    }
}
void list_for_each_rev(struct list *list, u32 (*func)(u32, void *))
{
    if (list == NULL)
    {
        return;
    }

    struct list_element *current = list->tail;

    u32 index = 0;

    while (current != NULL)
    {
        if (func(index, current->data) == 0)
        {
            break;
        }

        current = current->prev;
        ++index;
    }
}

void list_destroy(struct list *list, void (*free_func)(void *))
{
    if (list == NULL)
    {
        return;
    }

    struct list_element *current = list->head;

    while (current != NULL)
    {
        free_func(current->data);
        kfree(current);
        current = current->next;
    }

    kfree(list);
}

void list_remove_all(struct list *list, void *element)
{
    if (list == NULL)
    {
        return;
    }

    struct list_element *current = list->head;

    while (current != NULL)
    {
        if (current->data == element)
        {
            /** It's the head of the list **/
            if (current->prev == NULL)
            {
                list->head = current->next;
            }
            else
            {
                current->prev->next = current->next;
            }

            /** It's the tail of the list **/
            if (current->next == NULL)
            {
                list->tail = current->prev;
            }
            else
            {
                current->next->prev = current->prev;
            }

            kfree(current);
        }

        current = current->next;
    }
}