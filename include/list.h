#ifndef LIST_H_
#define LIST_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct list {
    void **array;
    int count;
    int capacity;
    int obj_size;
} list_t;

list_t *list_create(int capacity, int obj_size);
void list_append(list_t *const l, void *data);
void list_insert(list_t *const l, void *data, int index);
void list_remove(list_t *const l, int index, void (*func_free_data)(void *));
bool list_contains(list_t *const l, void *data, bool (*func_cmp)(void *, void *));
void list_clear(list_t *const l, void (*func_free_data)(void *));
void list_free(list_t *const l, void (*func_free_data)(void *));
void *list_get(list_t *const l, int index);

#endif
