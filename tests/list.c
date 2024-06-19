#include "list.h"
#include "../include/list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct fgh {
    int f;
    int g;
    int h;
} fgh_t;

void test_list_free_data(void *d) {
    fgh_t *data = d;

    free(d);
}

void test_list_print(list_t *l) {
    for (int i = 0; i < l->count; i++) {
        fgh_t *data = l->array[i];
        printf("%d\n", data->f);
    }
}
void test_list() {
    int obj_size = sizeof(fgh_t *);
    int capacity = 5;
    void **array = malloc(capacity * obj_size);
    list_t *list = list_create(capacity, obj_size);

    for (int i = 0; i < 5; i++) {
        fgh_t *data = malloc(obj_size);
        data->f = i;
        data->h = i;
        data->g = i;
        list_append(list, data);
    }

    fgh_t *data = list_get(list, 1);
    printf("list_get: %d\n", data->f);

    list_remove(list, 3, &test_list_free_data);

    test_list_print(list);

    list_free(list, &test_list_free_data);
}
