#include "../include/list.h"

void list_double_capacity(list_t *const l) {
    void **new_array = malloc(l->obj_size * l->capacity * 2);

    for (int i = 0; i < l->count; ++i) {
        new_array[i] = l->array[i];
    }

    free(l->array);

    l->array = new_array;
    l->capacity *= 2;
}

void list_append(list_t *const l, void *data) {
    if (l->count == l->capacity) {
        list_double_capacity(l);
    }

    l->array[l->count] = data;
    l->count++;
}

void list_insert(list_t *const l, void *data, int index) {
    if (index < 0 || index >= l->count) {
        printf("list_insert: Index(%d) out of bounds(%d)\n", index, l->count);
        return;
    }

    if (l->count == l->capacity) {
        list_double_capacity(l);
    }

    for (int i = l->count; i > index; i--) {
        l->array[i] = l->array[i - 1];
    }

    l->array[index] = data;
    l->count++;
}

void list_remove(list_t *const l, int index, void (*func_free_data)(void *)) {
    if (index < 0 || index >= l->count) {
        printf("list_remove: Index(%d) out of bounds(%d)\n", index, l->count);
        return;
    }

    void *data = l->array[index];
    if (func_free_data != NULL) {
        func_free_data(data);
    }
    data = NULL;

    for (int i = index + 1; i < l->count; i++) {
        l->array[i - 1] = l->array[i];

        if (i == l->count - 1) {
            l->array[i] = NULL;
        }
    }

    l->count--;
}

bool list_contains(list_t *const l, void *data, bool (*func_cmp)(void *, void *)) {
    for (int i = 0; i < l->count; i++) {
        if (func_cmp(l->array[i], data)) {
            return true;
        }
    }

    return false;
}

void list_clear(list_t *const l, void (*func_free_data)(void *)) {
    for (int i = 0; i < l->count; i++) {
        void *data = l->array[i];
        l->array[i] = NULL;

        if (func_free_data != NULL) {
            func_free_data(data);
        }
    }

    l->count = 0;
}

void list_free(list_t *const l, void (*func_free_data)(void *)) {
    list_clear(l, func_free_data);

    free(l->array);

    free(l);
}

void *list_get(list_t *const l, int index) {
    if (index >= l->count) {
        printf("list_remove: Index(%d) out of bounds(%d)\n", index, l->count);
        return NULL;
    }

    return l->array[index];
}

list_t *list_create(int capacity, int obj_size) {
    list_t *l = malloc(sizeof(list_t));
    if (l == NULL) {
        printf("Unable to malloc list\n");
        return NULL;
    }

    l->array = malloc(capacity * obj_size);
    l->count = 0;
    l->capacity = capacity;
    l->obj_size = obj_size;

    return l;
}
