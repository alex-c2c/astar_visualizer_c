#ifndef MINHEAP_H_
#define MINHEAP_H_

typedef struct minheap minheap_t;

minheap_t* heap_create(const int* const array, int capacity);
void heap_push(minheap_t* const h, int value);
int heap_pop(minheap_t* const h);
void heap_fix(minheap_t* const h);
void heap_print(const minheap_t* const h);
void heap_set(minheap_t* const h, int value, int index);

#endif
