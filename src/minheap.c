#include "../include/minheap.h"
#include <stdio.h>
#include <stdlib.h>

void minheap_swap_element(const minheap_t *h, const int a, const int b) {
    void *tmp = h->array[a];
    h->array[a] = h->array[b];
    h->array[b] = tmp;
}

void minheap_double_capacity(minheap_t *const h) {
    void **new_array = malloc(h->obj_size * h->capacity * 2);

    for (int i = 0; i < h->count; ++i) {
        new_array[i] = h->array[i];
    }

    free(h->array);

    h->array = new_array;
    h->capacity *= 2;
}

void minheap_sift_down(minheap_t *const h, int curr, const int end) {
    int left = (curr * 2) + 1;
    while (left <= end) {
        int right = left + 1;
        if (right > end) {
            right = -1;
        }

        int swap = left;
        if (right != -1 && h->func_cmp(h->array[left], h->array[right]) > 0) {
            swap = right;
        }

        if (h->func_cmp(h->array[swap], h->array[curr]) < 0) {
            minheap_swap_element(h, swap, curr);
            curr = swap;
            left = (curr * 2) + 1;
        } else {
            return;
        }
    }
}

void minheap_sift_up(minheap_t *const h) {
    int child = h->count - 1;
    int parent = (child - 1) / 2;

    while (child > 0 && h->func_cmp(h->array[child], h->array[parent]) < 0) {
        minheap_swap_element(h, child, parent);
        child = parent;
        parent = (child - 1) / 2;
    }
}

void minheap_fix(minheap_t *const h) {
    int end = h->count - 1;
    int last_non_leaf_index = (h->count - 2) / 2;

    for (int i = last_non_leaf_index; i >= 0; --i) {
        minheap_sift_down(h, i, end);
    }
}

minheap_t *minheap_create(const int obj_size, const int capacity, int (*func_cmp)(void *, void *)) {
    minheap_t *h = malloc(sizeof(minheap_t));
    if (h == NULL) {
        printf("[Error][minheap] minheap_create: Unable to malloc minheap\n");
        return NULL;
    }

    void **array = malloc(obj_size * capacity);
    if (array == NULL) {
        free(h);
        printf("[Error][minheap] minheap_create: Unable to malloc array\n");
        return NULL;
    }

    h->array = array;
    h->obj_size = obj_size;
    h->count = 0;
    h->capacity = capacity;
    h->func_cmp = func_cmp;

    minheap_fix(h);

    return h;
}

void minheap_push(minheap_t *const h, void *const value) {
    if (h->count == h->capacity) {
        minheap_double_capacity(h);
    }

    h->array[h->count] = value;
    h->count++;

    minheap_sift_up(h);
}

void *minheap_pop(minheap_t *const h) {
    if (h->count <= 0) {
        printf("[Error][minheap] minheap_pop: heap is empty\n");
        return NULL;
    }

    void *element = h->array[0];
    h->array[0] = NULL;

    minheap_swap_element(h, 0, h->count - 1);

    h->count--;

    minheap_sift_down(h, 0, h->count - 1);

    return element;
}

void *minheap_get(minheap_t *const h, const int index) {
    if (index < 0 || index >= h->count) {
        printf("[Error][minheap] minheap_get: Index(%d) out of bounds(%d)\n", index, h->count);
        return NULL;
    }

    return h->array[index];
}

void minheap_set(minheap_t *const h, void *const value, const int index) {
    if (index < 0 || index >= h->count) {
        printf("[Error][minheap] minheap_set: Index(%d) out of bounds(%d)\n", index, h->count);
        return;
    }

    h->array[index] = value;
    minheap_fix(h);
}

void minheap_clear(minheap_t *const h, void (*func_free_data)(void *)) {
    for (int i = 0; i < h->count; i++) {
        if (func_free_data != NULL) {
            func_free_data(h->array[i]);
        }

        h->array[i] = NULL;
    }

    h->count = 0;
}

void minheap_free(minheap_t *const h, void (*func_free_data)(void *)) {
    minheap_clear(h, func_free_data);

    free(h->array);

    free(h);
}
