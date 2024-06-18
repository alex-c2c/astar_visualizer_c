#ifndef ASTAR_H_
#define ASTAR_H_

typedef struct pt {
    int x;
    int y;
} pt_t;

typedef struct pf_node {
    pt_t* pt;
    pf_node* parent;
    int f;
    int g;
    int h;
} pf_node_t;

void start_path_finding(int col_size, int row_size, int blocker_size,
                        pt_t** blockers, int*** grid);

#endif
