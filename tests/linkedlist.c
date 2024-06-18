#include "linkedlist.h"
#include "../include/linkedlist.h"
#include <stdio.h>
#include <stdlib.h>

void test_ll_print(const linkedlist_t* const ll) {
    if (ll->count <= 0) {
        printf("test_ll_print: linkedlist is empty\n");
        return;
    }

    printf("test_ll_print: Printing all values(%d)...\n", ll->count);

    node_t* curr_node = ll->head;
    while (curr_node != NULL) {
        /*
         * cast data from (void*) to (long*)
         * then dereference pointer to get value in memory
         */
        printf("%ld\n", *(long*)(curr_node->data));
        curr_node = curr_node->next;
    }

    printf("test_ll_print: End\n");
}

long* test_ll_create_long_pointer(long number) {
    long* pt = malloc(sizeof(long));
    *pt = number;

    return pt;
}

void test_ll_free(linkedlist_t* ll) {
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

void test_linkedlist() {
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

    ll_insert_at(ll, test_ll_create_long_pointer(5), 5);

    for (int i = 0; i < 3; i++) {
        ll_insert_at(ll, test_ll_create_long_pointer(-1), 0);
    }

    ll_remove_head(ll);
    ll_remove_tail(ll);

    ll_insert_at(ll, test_ll_create_long_pointer(10), ll->count - 1);

    long* new_pt = test_ll_create_long_pointer(100);
    ll_insert_at(ll, new_pt, 2);

    test_ll_print(ll);

    void* another_pt = ll_get_data_at(ll, 2);
    printf("another_pt: %ld\n", *(long*)another_pt);

    ll_remove_data(ll, another_pt);
    test_ll_print(ll);

    test_ll_free(ll);
}
