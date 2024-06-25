#include "../include/astar.h"
#include "astar.h"
#include <stdint.h>

void test_astar() {
    int col_size = 5;
    int row_size = 5;
    int sx = 0;
    int sy = 0;
    int ex = 4;
    int ey = 4;

    list_t *return_path = malloc(sizeof(list_t));
    uint16_t return_path_create_result = list_create(return_path, 10, sizeof(pt_t));
    if (return_path_create_result != 0) {
        fprintf(stderr, "[Error][test_astar] test_astar: unable to create return_path\n");
        exit(return_path_create_result);
    }

    list_t *blockers = malloc(sizeof(list_t));
    uint16_t blockers_create_result = list_create(blockers, 10, sizeof(pt_t));
    if (blockers_create_result != 0) {
        fprintf(stderr, "[Error][test_astar] test_astar: unable to create blockers\n");
        exit(blockers_create_result);
    }

    list_append(blockers, astar_create_pt(4, 0));
    list_append(blockers, astar_create_pt(3, 1));
    list_append(blockers, astar_create_pt(2, 2));
    list_append(blockers, astar_create_pt(1, 3));
    // list_append(blockers, astar_create_pt(0, 4));
    uint16_t result = astar_start_path_finding(return_path, col_size, row_size, sx, sy, ex, ey, blockers);
    if (result != 0) {
        fprintf(stderr, "[Error][test_astar] test_astar: unable to finish path finding\n");
        exit(result);
    }

    if (return_path->count > 0) {
        printf("test_astar: Displaying return path!\n");
        for (int i = 0; i < return_path->count; i++) {
            pt_t *pt = list_get(return_path, i);
            printf("%d, %d\n", pt->x, pt->y);
        }
    } else {
        printf("test_astar: unable to get a return path...\n");
    }

    list_free(return_path, &astar_free_pt);
    list_free(blockers, &astar_free_pt);
}
