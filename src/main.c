#include "../include//linkedlist.h"
#include "../include/minheap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void test_heap(void) {
    int* array = (int*)malloc(sizeof(int) * 10);

    srand(time(NULL));

    printf("setting up first array.\n");
    for (int i = 0; i < 10; ++i) {
        int rand_num = rand() % 100;
        array[i] = rand_num;
        printf("%d\n", rand_num);
    }

    printf("\n\nsetting up heap\n");
    minheap_t* heap = heap_create(array, 10);

    free(array);

    heap_print(heap);

    heap_set(heap, -1, 9);

    printf("\nPopping heap...\n");
    for (int i = 0; i < 10; ++i) {
        int num = heap_pop(heap);
        printf("n: %d\n", num);
    }
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

int main(void) { test_linkedlist(); }
