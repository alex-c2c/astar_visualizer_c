#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

typedef struct node {
    struct node *prev;
    struct node *next;
    void *data;
} node_t;

typedef struct linkedlist {
    node_t *head;
    node_t *tail;
    int count;
} linkedlist_t;

linkedlist_t *ll_create();

void *ll_get_data_at(linkedlist_t *const ll, int index);

int ll_insert_head(linkedlist_t *const ll, void *data);
int ll_insert_tail(linkedlist_t *const ll, void *data);
int ll_insert_at(linkedlist_t *const ll, void *const data, int index);

int ll_remove_head(linkedlist_t *const ll, void (*func_free_data)(void *));
int ll_remove_tail(linkedlist_t *const ll, void (*func_free_data)(void *));
int ll_remove_at(linkedlist_t *const ll, int index, void (*func_free_data)(void *));
int ll_remove_data(linkedlist_t *const ll, const void *const data, void (*func_free_data)(void *));

void ll_clear(linkedlist_t *const ll, void (*func_free_data)(void *));
void ll_free(linkedlist_t *const ll, void (*func_free_data)(void *));

#endif
