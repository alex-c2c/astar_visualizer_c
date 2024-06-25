#include "list.h"
#include "../include/errno.h"
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
    int obj_size = sizeof(fgh_t);
    int capacity = 5;
    /*
    void **array = malloc(capacity * sizeof(fgh_t*));
    if (array == NULL) {
        fprintf(stderr, "[Error][test_list] test_list: unable to malloc array\n");
        exit(ERROR_MALLOC_FAILED);
    }
    */

    list_t *list = malloc(sizeof(list_t));
    if (list == NULL) {
        fprintf(stderr, "[Error][test_list] test_list: unable to malloc list_t\n");
        exit(ERROR_MALLOC_FAILED);
    }

    uint16_t result = list_create(list, capacity, sizeof(fgh_t *));
    if (result != 0) {
        fprintf(stderr, "[Error][test_list] test_list: unable to create list_t\n");
        exit(result);
    }

    for (int i = 0; i < 10; i++) {
        fgh_t *data = malloc(sizeof(fgh_t));
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
    free(list);
}
