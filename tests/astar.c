#include "../include/astar.h"
#include "astar.h"

void test_astar() {
    int col_size = 5;
    int row_size = 5;
    int sx = 0;
    int sy = 0;
    int ex = 4;
    int ey = 4;

    list_t *return_path = list_create(10, sizeof(pt_t));
    list_t *blockers = list_create(10, sizeof(pt_t));
    list_append(blockers, astar_create_pt(4, 0));
    list_append(blockers, astar_create_pt(3, 1));
    list_append(blockers, astar_create_pt(2, 2));
    list_append(blockers, astar_create_pt(1, 3));
    list_append(blockers, astar_create_pt(0, 4));
    astar_start_path_finding(return_path, col_size, row_size, sx, sy, ex, ey, blockers);

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
