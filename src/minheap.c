#include "../include/minheap.h"
#include <stdio.h>
#include <stdlib.h>

struct minheap {
    int* array;   /* */
    int count;    /* */
    int capacity; /* */
};

void swap_element(const minheap_t* h, const int a, const int b) {
    int tmp = h->array[a];
    h->array[a] = h->array[b];
    h->array[b] = tmp;
}

void double_heap_capacity(minheap_t* const h) {
    int* new_array = malloc(sizeof(int) * h->capacity * 2);

    for (int i = 0; i < h->count; ++i) {
        new_array[i] = h->array[i];
    }

    free(h->array);

    h->array = new_array;
    h->capacity *= 2;
}

const int compare(const minheap_t* const h, const int a, const int b) {
    if (h->array[a] < h->array[b]) {
        return -1;
    } else if (h->array[a] > h->array[b]) {
        return 1;
    }

    return 0;
}

void sift_down(minheap_t* const h, int curr, const int end) {
    int left = (curr * 2) + 1;
    while (left <= end) {
        int right = left + 1;
        if (right > end) {
            right = -1;
        }

        int swap = left;
        if (right != -1 && compare(h, left, right) > 0) {
            swap = right;
        }

        if (compare(h, swap, curr) < 0) {
            swap_element(h, swap, curr);
            curr = swap;
            left = (curr * 2) + 1;
        } else {
            return;
        }
    }
}

void sift_up(minheap_t* const h) {
    int child = h->count - 1;
    int parent = (child - 1) / 2;

    while (child > 0 && compare(h, child, parent) == -1) {
        swap_element(h, child, parent);
        child = parent;
        parent = (child - 1) / 2;
    }
}

void heap_fix(minheap_t* const h) {
    int end = h->count - 1;
    int last_non_leaf_index = (h->count - 2) / 2;

    for (int i = last_non_leaf_index; i >= 0; --i) {
        sift_down(h, i, end);
    }
}

minheap_t* heap_create(const int* const array, const int capacity) {
    minheap_t* h = (minheap_t*)malloc(sizeof(minheap_t));

    if (h == NULL) {
        printf("Unable to malloc minheap\n");
        return NULL;
    }

    h->array = (int*)malloc(sizeof(int) * capacity);
    if (h->array == NULL) {
        printf("Unable to malloc an array\n");
        return NULL;
    }

    int i;
    for (i = 0; i < capacity; i++) {
        h->array[i] = array[i];
    }

    h->count = i;
    h->capacity = capacity;

    heap_fix(h);

    return h;
}

void heap_push(minheap_t* const h, const int value) {
    if (h->count == h->capacity) {
        double_heap_capacity(h);
    }

    h->array[h->count] = value;
    h->count++;

    sift_up(h);
}

int heap_pop(minheap_t* const h) {
    int element = h->array[0];

    swap_element(h, 0, h->count - 1);

    h->count--;

    sift_down(h, 0, h->count - 1);

    return element;
}

void heap_set(minheap_t* const h, const int value, const int index) {
    h->array[index] = value;
    heap_fix(h);
}

void heap_print(const minheap_t* const h) {
    printf("Printing heap...\n");
    for (int i = 0; i < h->count; ++i) {
        printf("%d\n", h->array[i]);
    }
}
