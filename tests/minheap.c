#include "minheap.h"
#include "../include/minheap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct fgh {
    int f;
    int g;
    int h;
} fgh_t;

void test_minheap_print(const minheap_t *const h) {
    printf("test_minheap_print: START\n");
    for (int i = 0; i < h->count; ++i) {
        fgh_t *obj = h->array[i];
        printf("%d\n", obj->f);
    }
    printf("test_minheap_print: END\n");
}

int test_minheap_compare(void *a, void *b) {
    fgh_t *obj_a = a;
    fgh_t *obj_b = b;

    if (obj_a->f < obj_b->f) {
        return -1;
    } else if (obj_a->f > obj_b->f) {
        return 1;
    }

    return 0;
}

void test_minheap_free_data(void *d) {
    fgh_t *data = d;

    free(d);
}

void test_minheap(void) {
    srand(time(NULL));

    int obj_size = sizeof(fgh_t);
    int capacity = 10;

    minheap_t *heap = heap_create(obj_size, capacity, &test_minheap_compare);

    for (int i = 200; i >= 0; i--) {
        fgh_t *new_obj = malloc(sizeof(fgh_t *));
        int rand_num = rand() % 10000;
        new_obj->f = rand_num;
        new_obj->h = rand_num * 10;
        new_obj->g = rand_num % 100;

        heap_push(heap, new_obj);
    }

    for (int i = 0; i < 10; i++) {
        fgh_t *obj = heap_pop(heap);
        printf("%d\n", obj->f);
    }

    heap_clear(heap, &test_minheap_free_data);

    test_minheap_print(heap);

    heap_free(heap, &test_minheap_free_data);
}
