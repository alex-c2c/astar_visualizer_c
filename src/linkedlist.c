#include "../include/linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void ll_free_node(node_t* node) {
    node->next = NULL;
    node->prev = NULL;
    node->data = NULL;

    free(node);
}

node_t* ll_get_node_at(linkedlist_t* const ll, const int index) {
    if (index >= ll->count) {
        printf("ll_get_node_at: Index(%d) out of bounds(%d)\n", index,
               ll->count);
        return NULL;
    }

    node_t* curr_node = ll->head;
    int curr = 0;

    while (curr_node != NULL) {
        if (curr == index) {
            return curr_node;
        }

        curr_node = curr_node->next;
        curr++;
    }

    return NULL;
}

linkedlist_t* ll_create() {
    linkedlist_t* ll = malloc(sizeof(linkedlist_t));
    ll->head = NULL;
    ll->tail = NULL;
    ll->count = 0;

    return ll;
}

void* ll_get_data_at(linkedlist_t* const ll, const int index) {
    node_t* curr_node = ll_get_node_at(ll, index);
    if (curr_node == NULL) {
        printf("ll_get_data_at: Index(%d) out of bounds(%d)\n", index,
               ll->count);
        return NULL;
    }

    return curr_node->data;
}

int ll_insert_head(linkedlist_t* const ll, void* const data) {
    node_t* new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        printf("ll_insert_head: Unable to create new node\n");
        return -1;
    }

    new_node->data = data;

    if (ll->head == NULL) {
        ll->head = new_node;
        ll->tail = new_node;
    } else {
        node_t* next_node = ll->head;
        next_node->prev = new_node;
        new_node->next = next_node;
        ll->head = new_node;
    }

    ll->count++;

    return 0;
}

int ll_insert_tail(linkedlist_t* const ll, void* const data) {
    node_t* new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        printf("ll_insert_tail: Unable to create new node\n");
        return -1;
    }

    new_node->data = data;

    if (ll->tail == NULL) {
        ll->tail = new_node;
        ll->head = new_node;
    } else {
        node_t* prev_node = ll->tail;
        prev_node->next = new_node;
        new_node->prev = prev_node;
        ll->tail = new_node;
    }

    ll->count++;

    return 0;
}

int ll_insert_at(linkedlist_t* const ll, void* const data, const int index) {
    node_t* curr_node = ll_get_node_at(ll, index);
    if (curr_node == NULL) {
        printf("ll_insert: Index(%d) out of bounds(%d)\n", index, ll->count);
        return -1;
    }

    node_t* new_node = malloc(sizeof(node_t));
    if (new_node == NULL) {
        printf("ll_insert_at: Unable to create new node\n");
        return -1;
    }

    new_node->data = data;

    if (curr_node->prev == NULL) { /* Insert at head */
        new_node->next = curr_node;
        curr_node->prev = new_node;
        ll->head = new_node;
    } else if (curr_node->next == NULL) { /* Insert at tail */
        new_node->prev = curr_node;
        curr_node->next = new_node;
        ll->tail = new_node;
    } else { /* Insert inbetween linked list */
        node_t* prev_node = curr_node->prev;
        prev_node->next = new_node;
        new_node->prev = prev_node;
        new_node->next = curr_node;
        curr_node->prev = new_node;
    }

    ll->count++;

    return 0;
}

int ll_remove_node(linkedlist_t* const ll, node_t* dn) {
    if (dn == NULL) {
        printf("ll_remove_node: node to be deleted is NULL.\n");
        return -1;
    }

    if (dn->prev == NULL &&
        dn->next == NULL) { /* remove the last node from linkedlist */
        ll->head = NULL;
        ll->tail = NULL;
    } else if (dn->prev == NULL) { /* remove head */
        node_t* new_node = dn->next;
        new_node->prev = NULL;
        ll->head = new_node;
    } else if (dn->next == NULL) { /* remove tail */
        node_t* prev_node = dn->prev;
        prev_node->next = NULL;
        ll->tail = prev_node;
    } else { /* remove from middle of linkedlist */
        node_t* next_node = dn->next;
        node_t* prev_node = dn->prev;
        prev_node->next = next_node;
        next_node->prev = prev_node;
    }

    ll_free_node(dn);

    ll->count--;

    return 0;
}

int ll_remove_head(linkedlist_t* const ll) {
    if (ll->count <= 0) {
        printf("ll_remove_head: linked list is empty.\n");
        return -1;
    }

    ll_remove_node(ll, ll->head);

    return 0;
}

int ll_remove_tail(linkedlist_t* const ll) {
    if (ll->count <= 0) {
        printf("ll_remove_tail: linked list is empty.\n");
        return -1;
    }

    ll_remove_node(ll, ll->tail);

    return 0;
}

int ll_remove_at(linkedlist_t* const ll, const int index) {
    node_t* dn = ll_get_node_at(ll, index);
    if (dn == NULL) {
        printf("ll_remove_at: Index(%d) out of bounds(%d)\n", index, ll->count);
        return -1;
    }

    ll_remove_node(ll, dn);

    return 0;
}

int ll_remove_data(linkedlist_t* const ll, const void* const data) {
    if (ll->count <= 0) {
        printf("ll_remove_data: linked list is empty.\n");
        return -1;
    }

    node_t* curr_node = ll->head;
    while (curr_node != NULL) {
        if (curr_node->data == data) {
            ll_remove_node(ll, curr_node);
            return 0;
        }
        curr_node = curr_node->next;
    }

    return -1;
}

void ll_clear(linkedlist_t* const ll) {
    node_t* curr_node = ll->head;
    while (curr_node->next != NULL) {
        node_t* dn = curr_node;
        curr_node = curr_node->next;

        ll_free_node(dn);
    }

    ll->head = NULL;
    ll->tail = NULL;
    ll->count = 0;
}
