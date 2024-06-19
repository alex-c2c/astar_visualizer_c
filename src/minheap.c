#include "../include/minheap.h"
#include <stdio.h>
#include <stdlib.h>

void swap_element(const minheap_t *h, const int a, const int b) {
    void *tmp = h->array[a];
    h->array[a] = h->array[b];
    h->array[b] = tmp;
}

void double_heap_capacity(minheap_t *const h) {
    void **new_array = malloc(h->obj_size * h->capacity * 2);

    for (int i = 0; i < h->count; ++i) {
        new_array[i] = h->array[i];
    }

    free(h->array);

    h->array = new_array;
    h->capacity *= 2;
}

void sift_down(minheap_t *const h, int curr, const int end) {
    int left = (curr * 2) + 1;
    while (left <= end) {
        int right = left + 1;
        if (right > end) {
            right = -1;
        }

        int swap = left;
        if (right != -1 && h->compare_func(h->array[left], h->array[right]) > 0) {
            swap = right;
        }

        if (h->compare_func(h->array[swap], h->array[curr]) < 0) {
            swap_element(h, swap, curr);
            curr = swap;
            left = (curr * 2) + 1;
        } else {
            return;
        }
    }
}

void sift_up(minheap_t *const h) {
    int child = h->count - 1;
    int parent = (child - 1) / 2;

    while (child > 0 && h->compare_func(h->array[child], h->array[parent]) < 0) {
        swap_element(h, child, parent);
        child = parent;
        parent = (child - 1) / 2;
    }
}

void heap_fix(minheap_t *const h) {
    int end = h->count - 1;
    int last_non_leaf_index = (h->count - 2) / 2;

    for (int i = last_non_leaf_index; i >= 0; --i) {
        sift_down(h, i, end);
    }
}

minheap_t *heap_create(const int obj_size, const int capacity, int (*compare_func)(void *, void *)) {
    minheap_t *h = malloc(sizeof(minheap_t));
    if (h == NULL) {
        printf("Unable to malloc minheap\n");
        return NULL;
    }

    void **array = malloc(obj_size * capacity);
    if (array == NULL) {
        free(h);
        printf("Unable to malloc array in minheap_t\n");
        return NULL;
    }

    h->array = array;
    h->obj_size = obj_size;
    h->count = 0;
    h->capacity = capacity;
    h->compare_func = compare_func;

    heap_fix(h);

    return h;
}

void heap_push(minheap_t *const h, void *const value) {
    if (h->count == h->capacity) {
        double_heap_capacity(h);
    }

    h->array[h->count] = value;
    h->count++;

    sift_up(h);
}

void *heap_pop(minheap_t *const h) {
    void *element = h->array[0];
    h->array[0] = NULL;

    swap_element(h, 0, h->count - 1);

    h->count--;

    sift_down(h, 0, h->count - 1);

    return element;
}

void heap_set(minheap_t *const h, void *const value, const int index) {
    h->array[index] = value;
    heap_fix(h);
}

void heap_clear(minheap_t *const h, void (*free_data)(void *)) {
    for (int i = 0; i < h->count; i++) {
        if (free_data != NULL) {
            free_data(h->array[i]);
        }

        h->array[i] = NULL;
    }

    h->count = 0;
}

void heap_free(minheap_t *const h, void (*free_data)(void *)) {
    heap_clear(h, free_data);

    free(h->array);

    free(h);
}
