#include "../include//linkedlist.h"
#include "../include/minheap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct fgh {
    int f;
    int g;
    int h;
} fgh_t;

int compare(void* a, void* b) {
    fgh_t* obj_a = a;
    fgh_t* obj_b = b;

    if (obj_a->f < obj_b->f) {
        return -1;
    } else if (obj_a->f > obj_b->f) {
        return 1;
    }

    return 0;
}

void free_data(void* d) {
    fgh_t* data = d;

    free(d);
}

void test_heap(void) {
    srand(time(NULL));

    void** array = malloc(sizeof(fgh_t*) * 10);

    minheap_t* heap = heap_create(array, sizeof(fgh_t*), 0, 10, &compare);

    for (int i = 200; i >= 0; i--) {
        fgh_t* new_obj = malloc(sizeof(fgh_t*));
        int rand_num = rand() % 10000;
        new_obj->f = rand_num;
        new_obj->h = rand_num * 10;
        new_obj->g = rand_num % 100;

        heap_push(heap, new_obj);
    }

    for (int i = 0; i < 10; i++) {
        fgh_t* obj = heap_pop(heap);
        printf("%d\n", obj->f);
    }

    heap_clear(heap, &free_data);

    heap_print(heap);

    free(heap->array);
    heap->array = NULL;

    free(heap);
}

void heap_print(const minheap_t* const h) {
    printf("heap_print: START\n");
    for (int i = 0; i < h->count; ++i) {
        fgh_t* obj = h->array[i];
        printf("%d\n", obj->f);
    }
    printf("heap_print: END\n");
}

void test_print_linkedlist(const linkedlist_t* const ll) {
    if (ll->count <= 0) {
        printf("test_print_linkedlist: linkedlist is empty\n");
        return;
    }

    printf("test_print_linkedlist: Printing all values(%d)...\n", ll->count);

    node_t* curr_node = ll->head;
    while (curr_node != NULL) {
        /*
         * cast data from (void*) to (long*)
         * then dereference pointer to get value in memory
         */
        printf("%ld\n", *(long*)(curr_node->data));
        curr_node = curr_node->next;
    }

    printf("test_print_linkedlist: End\n");
}

long* create_long_pointer(long number) {
    long* pt = malloc(sizeof(long));
    *pt = number;

    return pt;
}

void free_linkedlist(linkedlist_t* ll) {
    node_t* curr_node = ll->head;
    while (curr_node != NULL) {
        node_t* del_node = curr_node;
        curr_node = curr_node->next;

        del_node->prev = NULL;
        del_node->next = NULL;

        free(del_node->data);
        del_node->data = NULL;

        free(del_node);
        del_node = NULL;
    }

    ll->head = NULL;
    ll->tail = NULL;
    ll->count = 0;

    free(ll);
    ll = NULL;
}

void test_linkedlist(void) {
    linkedlist_t* ll = ll_create();

    for (int i = 0; i < 10; i++) {
        long* num_pt = malloc(sizeof(long));
        /* dereference num_pt, then set the value of (long)i to it */
        *num_pt = (long)i;
        ll_insert_tail(ll, num_pt);
    }

    ll_remove_at(ll, 5);

    long* nn_pt1 = malloc(sizeof(long));
    *nn_pt1 = 10;
    ll_insert_tail(ll, nn_pt1);

    ll_insert_at(ll, create_long_pointer(5), 5);

    for (int i = 0; i < 3; i++) {
        ll_insert_at(ll, create_long_pointer(-1), 0);
    }

    ll_remove_head(ll);
    ll_remove_tail(ll);

    ll_insert_at(ll, create_long_pointer(10), ll->count - 1);

    long* new_pt = create_long_pointer(100);
    ll_insert_at(ll, new_pt, 2);

    test_print_linkedlist(ll);

    void* another_pt = ll_get_data_at(ll, 2);
    printf("another_pt: %ld\n", *(long*)another_pt);

    ll_remove_data(ll, another_pt);
    test_print_linkedlist(ll);

    free_linkedlist(ll);
}

typedef struct pt {
    int x;
    int y;
} pt_t;

void test_print_array(int c, int r, pt_t*** array) {
    for (int j = 0; j < r; j++) {
        for (int i = 0; i < c; i++) {
            pt_t* pt = array[j][i];
            printf("i: %d, j: %d, pt: %d, %d\n", i, j, pt->x, pt->y);
        }
    }
}

void test_array_pointer() {
    /*
    int count = 0;
    int array[5][5] = {{0}};
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            array[i][j] = count;
            count++;
        }
    }
    */

    int c = 5;
    int r = 5;

    pt_t*** array = malloc(sizeof(pt_t**) * r);
    for (int i = 0; i < r; i++) {
        pt_t** col = malloc(sizeof(pt_t*) * c);
        array[i] = col;
    }

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            pt_t* pt = malloc(sizeof(pt_t));
            pt->x = j;
            pt->y = i;
            array[j][i] = pt;
        }
    }

    test_print_array(c, r, array);

    for (int j = 0; j < r; j++) {
        for (int i = 0; i < c; i++) {
            pt_t* pt = array[j][i];
            free(pt);
        }

        free(array[j]);
    }

    free(array);
}

int main(void) {
    // test_linkedlist();

    // test_array_pointer();

    test_heap();
}
