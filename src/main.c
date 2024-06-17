#include "../include/minheap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
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
