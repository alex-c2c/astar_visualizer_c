#ifndef MINHEAP_H_
#define MINHEAP_H_

typedef struct minheap {
    void **array;
    int count;
    int capacity;
    int obj_size;
    int (*compare_func)(void *, void *);
} minheap_t;

minheap_t *heap_create(int obj_size, int capacity, int (*compare_func)(void *, void *));
void heap_push(minheap_t *const h, void *const value);
void *heap_pop(minheap_t *const h);
void heap_fix(minheap_t *const h);
void heap_print(const minheap_t *const h);
void heap_set(minheap_t *const h, void *const value, int index);
void heap_clear(minheap_t *const h, void (*free_data)(void *));
void heap_free(minheap_t *const h, void (*free_data)(void *));

#endif
