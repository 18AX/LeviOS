
#ifndef LINKED_LIST_HEADER
#define LINKED_LIST_HEADER

#include <levi/types.h>

struct linked_list_elt
{
    struct linked_list_elt *prev;
    struct linked_list_elt *next;
    void *value;
};

typedef struct
{
    struct linked_list_elt *head;
    struct linked_list_elt *tail;
    u64 size;
    void (*free_func)(void *ptr);
} linked_list_t;

/**
 * @brief create a new empty linked list.
 *
 * @param free_func function use to free elements in the list when the list is
 * free. If you do not want the elements are freed, pass NULL as argument.
 * @return linked_list_t*
 */
linked_list_t *linked_list_new(void (*free_func)(void *));

/**
 * @brief Add an element to the linked list
 *
 * @param list
 * @param elt
 * @return u32 1 on succes, 0 on error.
 */
u32 linked_list_add(linked_list_t *list, void *elt);

/**
 * @brief Get the element at the specified index
 *
 * @param list
 * @param index
 * @return void* the value on success, NULL on error.
 */
void *linked_list_get(linked_list_t *list, u32 index);

/**
 * @brief Pop the last element of the list.
 *
 * @param list
 * @return void* the value on success, NULL on error.
 */
void *linked_list_pop_tail(linked_list_t *list);

/**
 * @brief Pop the first element of the list.
 *
 * @param list
 * @return void* the value on succes, NULL on error.
 */
void *linked_list_pop_front(linked_list_t *list);

/**
 * @brief Remove the element at the spciefied index.
 *
 * @param list
 * @param index
 * @return u32 1 on success, 0 on error.
 */
u32 linked_list_remove_at(linked_list_t *list, u32 index);

/**
 * @brief Remove all the elements that match the equal function (equal function
 * must return 1 on value equal).
 *
 * @param list
 * @param eq_func
 * @return u32 number of elements removed.
 */
u32 linked_list_remove_eq(linked_list_t *list, u32 (*eq_func)(void *));

/**
 * @brief Remove all pointers matching the same address
 *
 * @param list
 * @param ptr
 * @return u32 number of elements removed
 */
u32 linked_list_remove_ptr(linked_list_t *list, void *ptr);

/**
 * @brief Call the specified function on each elements.
 *
 * @param list
 * @param func
 */
void linked_list_for_each(linked_list_t *list, void (*func)(u32, void *value));

/**
 * @brief Call the specified function on each elements (reverse order).
 *
 * @param list
 * @param func
 */
void linked_list_for_each_rev(linked_list_t *list,
                              void (*func)(u32, void *value));

/**
 * @brief Get the size of the linked list (number of elements).
 *
 * @param list
 * @return u64
 */
u64 linked_list_size(linked_list_t *list);

/**
 * @brief Free a linked list. If a free func was specified at the list creation,
 * all values will be freed.
 *
 * @param list
 */
void linked_list_free(linked_list_t *list);

#endif