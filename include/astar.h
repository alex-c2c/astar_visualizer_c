#ifndef ASTAR_H_
#define ASTAR_H_

#include "list.h"

typedef struct pt {
    int x;
    int y;
} pt_t;

typedef struct pf_node {
    pt_t *pt;
    struct pf_node *parent;
    int f;
    int g;
    int h;
} pf_node_t;

void astar_start_path_finding(list_t *return_path, int col_size, int row_size, int sx, int sy, int ex, int ey, list_t *blockers);
void astar_free_pt(void *d);
pt_t *astar_create_pt(int x, int y);

#endif
