#include "../include/astar.h"
#include "../include/list.h"
#include "../include/minheap.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

pt_t *astar_create_pt(int x, int y) {
    pt_t *pt = malloc(sizeof(pt_t));
    if (pt == NULL) {
        printf("[Error][astar] astar_create_pt: unable to malloc pt_t\n");
        return NULL;
    }

    pt->x = x;
    pt->y = y;

    return pt;
}

pf_node_t *astar_create_node(int x, int y, pf_node_t *parent, int f, int g, int h) {
    pt_t *pt = astar_create_pt(x, y);
    if (pt == NULL) {
        printf("[Error][astar] astar_create_node: unable to malloc pt_t\n");
        return NULL;
    }

    pf_node_t *node = malloc(sizeof(pf_node_t));
    if (node == NULL) {
        printf("[Error][astar] astar_create_node: unable to malloc pf_node_t\n");
        return NULL;
    }

    node->pt = pt;
    node->parent = parent;
    node->f = f;
    node->h = h;
    node->g = g;

    return node;
}

bool astar_is_pt_equal(pt_t *a, pt_t *b) { return a->x == b->x && a->y == b->y; }

bool astar_comparator_pt(void *d_a, void *d_b) {
    pt_t *pt_a = d_a;
    pt_t *pt_b = d_b;

    return astar_is_pt_equal(pt_a, pt_b);
}

bool astar_comparator_node_pt(void *d_node, void *d_pt) {
    pf_node_t *node = d_node;
    pt_t *pt = d_pt;

    return astar_is_pt_equal(node->pt, pt);
}

int astar_comparator_heap(void *a, void *b) {
    pf_node_t *na = a;
    pf_node_t *nb = b;

    if (na->f < nb->f) {
        return -1;
    } else if (na->f > nb->f) {
        return 1;
    }

    return 0;
}

void astar_free_node(void *d) {
    free(((pf_node_t *)d)->pt);
    free(d);
}

void astar_free_pt(void *d) { free(d); }

int astar_square(pt_t *a, pt_t *b) {
    int c = a->x - b->x;
    int d = a->y - b->y;

    return c * c + d * d;
}

bool astar_is_diagonal(pt_t *a, pt_t *b) {
    int diff_x = a->x - b->x;
    int diff_y = a->y - b->y;

    return diff_x != 0 && diff_y != 0;
}

void astar_set_return_path(list_t *return_path, pf_node_t *node) {
    if (return_path == NULL) {
        printf("[Error][Astar] astar_set_return_path: return_path is NULL \n");
        return;
    }

    pf_node_t *curr_node = node;
    while (curr_node != NULL) {
        list_append(return_path, astar_create_pt(curr_node->pt->x, curr_node->pt->y));
        curr_node = curr_node->parent;
    }
}

void astar_set_valid_pts(list_t *valid_pts, pf_node_t *curr_node, pt_t *steps[8], list_t *blockers, int col_size, int row_size) {
    /*
     * index 0: T
     * index 1: R
     * index 2: B
     * index 3: L
     * index 4: TR
     * index 5: BR
     * index 6: BL
     * index 7: TL
     */
    bool blocked_array[8] = {false};

    for (int i = 0; i < 8; i++) {
        int nx = curr_node->pt->x + steps[i]->x;
        int ny = curr_node->pt->y + steps[i]->y;

        if (nx < 0 || nx >= col_size || ny < 0 || ny >= row_size) {
            continue;
        }

        pt_t *new_pt = astar_create_pt(nx, ny);

        if (list_contains(blockers, new_pt, &astar_comparator_pt)) {
            printf("found blocker: %d, %d\n", new_pt->x, new_pt->y);
            blocked_array[i] = true;
            astar_free_pt(new_pt);
            continue;
        }

        /* check for diagonal blockage from adj pts */
        switch (i) {
        case 4: {
            if (blocked_array[0] && blocked_array[1]) {
                continue;
            }
            break;
        }
        case 5: {
            if (blocked_array[1] && blocked_array[2]) {
                continue;
            }
            break;
        }
        case 6: {
            if (blocked_array[2] && blocked_array[3]) {
                continue;
            }
            break;
        }
        case 7: {
            if (blocked_array[0] && blocked_array[3]) {
                continue;
            }
            break;
        }
        default: {
            break;
        }
        }

        list_append(valid_pts, astar_create_pt(nx, ny));
    }
}

pf_node_t *astar_get_existing_node(minheap_t *heap, pt_t *pt) {
    if (heap == NULL) {
        return NULL;
    }

    for (int i = 0; i < heap->count; i++) {
        pf_node_t *node = heap_get(heap, i);
        if (node->pt->x == pt->x && node->pt->y == pt->y) {
            return node;
        }
    }

    return NULL;
}

pt_t **astar_create_steps() {
    pt_t **steps = malloc(8 * sizeof(pt_t *));
    steps[0] = astar_create_pt(0, -1);
    steps[1] = astar_create_pt(1, 0);
    steps[2] = astar_create_pt(0, 1);
    steps[3] = astar_create_pt(-1, 0);
    steps[4] = astar_create_pt(1, -1);
    steps[5] = astar_create_pt(1, 1);
    steps[6] = astar_create_pt(-1, 1);
    steps[7] = astar_create_pt(-1, -1);

    return steps;
}

void free_steps(pt_t **steps) {
    for (int i = 0; i < 8; i++) {
        free(steps[i]);
    }

    free(steps);

    steps = NULL;
}

void astar_free_all(pt_t **steps, list_t *close_list, minheap_t *open_heap, pt_t *end_pt) {
    list_free(close_list, &astar_free_node);
    heap_free(open_heap, &astar_free_node);
    free_steps(steps);
    astar_free_pt(end_pt);
}

void astar_start_path_finding(list_t *return_path, int col_size, int row_size, int sx, int sy, int ex, int ey, list_t *blockers) {
    pf_node_t *start_node = astar_create_node(sx, sy, NULL, 0, 0, 0);
    pt_t *end_pt = astar_create_pt(ex, ey);

    pt_t **steps = astar_create_steps();

    list_t *close_list = list_create(10, sizeof(pf_node_t));
    minheap_t *open_heap = heap_create(sizeof(pf_node_t), 10, &astar_comparator_heap);
    heap_push(open_heap, start_node);

    while (open_heap->count > 0) {
        pf_node_t *curr_node = heap_pop(open_heap);

        if (astar_is_pt_equal(curr_node->pt, end_pt)) {
            astar_set_return_path(return_path, curr_node);

            astar_free_all(steps, close_list, open_heap, end_pt);

            return;
        }

        list_append(close_list, curr_node);

        list_t *valid_pts = list_create(10, sizeof(pt_t));
        astar_set_valid_pts(valid_pts, curr_node, steps, blockers, col_size, row_size);

        for (int i = 0; i < valid_pts->count; i++) {
            pt_t *pt = list_get(valid_pts, i);

            if (list_contains(close_list, pt, &astar_comparator_node_pt)) {
                continue;
            }

            int g = 0;
            if (astar_is_diagonal(pt, curr_node->pt)) {
                g = curr_node->g + 15;
            } else {
                g = curr_node->g + 10;
            }

            int h = astar_square(pt, end_pt);
            int f = g + h;

            pf_node_t *existing_node = astar_get_existing_node(open_heap, pt);
            if (existing_node != NULL) {
                if (existing_node->g > g) {
                    existing_node->g = g;
                    existing_node->f = f;
                    existing_node->h = h;
                    existing_node->parent = curr_node;
                    heap_fix(open_heap);
                }
                continue;
            }

            pf_node_t *new_node = astar_create_node(pt->x, pt->y, curr_node, f, g, h);
            heap_push(open_heap, new_node);
        }

        list_free(valid_pts, &astar_free_pt);
        valid_pts = NULL;
    }

    astar_free_all(steps, close_list, open_heap, end_pt);

    return;
}
