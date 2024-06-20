#ifndef MINminheap_H_
#define MINminheap_H_

typedef struct minheap {
    void **array;
    int count;
    int capacity;
    int obj_size;
    int (*func_cmp)(void *, void *);
} minheap_t;

minheap_t *minheap_create(int obj_size, int capacity, int (*func_cmp)(void *, void *));
void minheap_push(minheap_t *const h, void *const value);
void *minheap_pop(minheap_t *const h);
void *minheap_get(minheap_t *const h, int index);
void minheap_fix(minheap_t *const h);
void minheap_print(const minheap_t *const h);
void minheap_set(minheap_t *const h, void *const value, int index);
void minheap_clear(minheap_t *const h, void (*func_free_data)(void *));
void minheap_free(minheap_t *const h, void (*func_free_data)(void *));

#endif
