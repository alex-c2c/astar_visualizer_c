#include "array.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct pt {
    int x;
    int y;
} pt_t;

void test_array_print(int c, int r, pt_t*** array) {
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

    test_array_print(c, r, array);

    for (int j = 0; j < r; j++) {
        for (int i = 0; i < c; i++) {
            pt_t* pt = array[j][i];
            free(pt);
        }

        free(array[j]);
    }

    free(array);
}
