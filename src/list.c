#include "../include/list.h"
#include "../include/errno.h"
#include <stdint.h>

uint16_t list_double_capacity(list_t *const l) {
    void **new_array = malloc(l->obj_size * l->capacity * 2);
    if (new_array == NULL) {
        fprintf(stderr, "[Error][list] list_double_capacity: unable to malloc new_array\n");
        return ERROR_MALLOC_FAILED;
    }

    for (int i = 0; i < l->count; ++i) {
        new_array[i] = l->array[i];
    }

    free(l->array);

    l->array = new_array;
    l->capacity *= 2;

    return 0;
}

uint16_t list_append(list_t *const l, void *data) {
    if (l->count == l->capacity) {
        uint16_t result = list_double_capacity(l);
        if (result != 0) {
            return result;
        }
    }

    l->array[l->count] = data;
    l->count++;

    return 0;
}

uint16_t list_insert(list_t *const l, void *data, int index) {
    if (index < 0 || index >= l->count) {
        fprintf(stderr, "[Error][list] list_insert: Index(%d) out of bounds(%d)\n", index, l->count);
        return ERROR_LIST_OUT_OF_BOUNDS;
    }

    if (l->count == l->capacity) {
        uint16_t result = list_double_capacity(l);
        if (result != 0) {
            return result;
        }
    }

    for (int i = l->count; i > index; i--) {
        l->array[i] = l->array[i - 1];
    }

    l->array[index] = data;
    l->count++;

    return 0;
}

uint16_t list_remove(list_t *const l, int index, void (*func_free_data)(void *)) {
    if (index < 0 || index >= l->count) {
        fprintf(stderr, "[Error][list] list_remove: Index(%d) out of bounds(%d)\n", index, l->count);
        return ERROR_LIST_OUT_OF_BOUNDS;
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

    return 0;
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
}

void *list_get(list_t *const l, int index) {
    if (index >= l->count) {
        printf("[Debug][list] list_remove: Index(%d) out of bounds(%d)\n", index, l->count);
        return NULL;
    }

    return l->array[index];
}

uint16_t list_create(list_t *list, int capacity, int obj_size) {
    list->array = malloc(capacity * obj_size);
    if (list->array == NULL) {
        fprintf(stderr, "[Error][list] Unable to malloc l->array\n");
        return ERROR_MALLOC_FAILED;
    }

    list->count = 0;
    list->capacity = capacity;
    list->obj_size = obj_size;

    return 0;
}
