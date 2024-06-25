#include "../include/astar.h"
#include "../include/file.h"
#include "../include/list.h"
#include "../include/rle.h"
/*
#include "../tests/array.h"
#include "../tests/astar.h"
#include "../tests/file.h"
#include "../tests/linkedlist.h"
#include "../tests/list.h"
#include "../tests/minheap.h"
#include "../tests/rle.h"
*/

#include "../include/errno.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void free_pt(void *d) { free(d); }

char ***get_decoded_output(char ***encoded_lines, size_t line_count) {
    char ***decoded_output = malloc(sizeof(char **));
    if (decoded_output == NULL) {
        fprintf(stderr, "[Error][main] get_decoded_output: unable to malloc decoded_output\n");
        exit(ERROR_MALLOC_FAILED);
    }

    uint16_t decode_result = rle_decode(encoded_lines, decoded_output, line_count);
    if (decode_result != 0) {
        fprintf(stderr, "[Error][main] get_decoded_output: decoding failed\n");
        exit(decode_result);
    }

    return decoded_output;
}

char ***get_encoded_content(char ***decoded_lines, size_t line_count) {
    char ***encoded_output = malloc(sizeof(char **));
    if (encoded_output == NULL) {
        fprintf(stderr, "[Error][main] get_encoded_output: unable to malloc encoded_output\n");
        exit(ERROR_MALLOC_FAILED);
    }

    uint16_t encode_result = rle_encode(decoded_lines, encoded_output, line_count);
    if (encode_result != 0) {
        fprintf(stderr, "[Error][main] get_encoded_output: encoding failed\n");
        exit(encode_result);
    }

    return encoded_output;
}

char ***get_file_content(char *file_path, size_t *line_count) {
    /* read from file */
    char ***file_content = malloc(sizeof(char **));
    if (file_content == NULL) {
        fprintf(stderr, "[Error][main] main: unable to malloc file_input\n");
        exit(ERROR_MALLOC_FAILED);
    }

    int16_t file_read_result = file_read(file_content, line_count, file_path);
    if (file_read_result != 0) {
        fprintf(stderr, "[Error][rle] test_rle: unable to read from file\n");
        exit(ERROR_FILE_READ);
    }

    return file_content;
}

list_t *create_empty_list(uint32_t capacity, uint32_t obj_size) {
    list_t *empty_list = malloc(sizeof(list_t));
    uint16_t result = list_create(empty_list, capacity, obj_size);
    if (result != 0) {
        fprintf(stderr, "[Error][test_astar] test_astar: unable to create return_path\n");
        exit(result);
    }

    return empty_list;
}

list_t *get_blockers(char ***grid, size_t col_size, size_t row_size) {
    if (grid == NULL) {
        exit(-1);
    }

    list_t *blockers = create_empty_list(8, sizeof(pt_t));
    for (size_t y = 0; y < row_size; y++) {
        for (size_t x = 0; x < col_size; x++) {
            if ((*grid)[y][x] == 'B') {
                pt_t *blocker = astar_create_pt((int)x, (int)y);
                if (blocker == NULL) {
                    fprintf(stderr, "[Error][main] get_blockers: unable to create blocker at %ld, %ld\n", x, y);
                    continue;
                }

                list_append(blockers, astar_create_pt(x, y));
            }
        }
    }

    return blockers;
}

pt_t *get_pt(char ***grid, char c, size_t col_size, size_t row_size) {
    if (grid == NULL) {
        fprintf(stderr, "[Error][main] get_start_pt: grid is NULL\n");
        exit(-1);
    }

    for (size_t y = 0; y < row_size; y++) {
        for (size_t x = 0; x < col_size; x++) {
            if ((*grid)[y][x] == c) {
                pt_t *start_pt = astar_create_pt(x, y);
                if (start_pt == NULL) {
                    fprintf(stderr, "[Error][main] get_start_pt: unable to create '%c' pt at %ld, %ld\n", c, x, y);
                    exit(-1);
                }

                return start_pt;
            }
        }
    }

    fprintf(stderr, "[Error][main] get_start_pt: unable to find a '%c' point in grid\n", c);
    exit(-1);
}

void apply_return_path_to_grid(list_t *return_path, char ***grid, size_t col_size, size_t row_size) {
    for (uint32_t i = 1; i < return_path->count - 1; i++) {
        pt_t *pt = list_get(return_path, i);
        if (pt == NULL) {
            fprintf(stderr, "[Error][main] apply_return_path_to_grid: unable to get return path at index '%d'\n", i);
            continue;
        }

        if (pt->x < 0 || pt->x >= col_size || pt->y < 0 || pt->y >= row_size) {
            fprintf(stderr, "[Error][main] apply_return_path_to_grid: pt(%d,%d) at index '%d' is out of bounds (c:%ld, r:%ld)\n", pt->x, pt->y, i, col_size, row_size);
            continue;
        }

        (*grid)[pt->y][pt->x] = 'P';
    }
}

int main(void) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("[Debug][file] test_file: cwd - '%s'\n", cwd);
    } else {
        fprintf(stderr, "[Error][file] test_file: unable to get cwd\n");
        exit(-1);
    }

    char input_file_path[PATH_MAX];
    sprintf(input_file_path, "%s/%s", cwd, "data/input_tile_data.txt");
    char output_file_path[PATH_MAX];
    sprintf(output_file_path, "%s/%s", cwd, "data/output_tile_data.txt");

    size_t file_line_count = 0;
    char ***input_file = get_file_content(input_file_path, &file_line_count);
    char ***decoded_output = get_decoded_output(input_file, file_line_count);

    size_t col_size = strlen((*decoded_output)[0]);
    size_t row_size = file_line_count;
    list_t *blockers = get_blockers(decoded_output, col_size, row_size);
    pt_t *start_pt = get_pt(decoded_output, 'S', col_size, row_size);
    pt_t *end_pt = get_pt(decoded_output, 'E', col_size, row_size);
    list_t *return_path = create_empty_list(8, sizeof(pt_t));

    uint16_t pf_result = astar_start_path_finding(return_path, col_size, row_size, start_pt->x, start_pt->y, end_pt->x, end_pt->y, blockers);
    if (pf_result != 0) {
        fprintf(stderr, "[Error][test_astar] test_astar: unable to finish path finding\n");
        exit(pf_result);
    }

    if (return_path->count == 0) {
        printf("[Debug][main] main: Unable to find return path, exiting\n");
        exit(0);
    }

    apply_return_path_to_grid(return_path, decoded_output, col_size, row_size);

    char ***encoded_output = get_encoded_content(decoded_output, file_line_count);

    int32_t file_write_result = file_write(*encoded_output, file_line_count, output_file_path);
    if (file_write_result != 0) {
        fprintf(stderr, "[Error][main] main: unable to write to file\n");
        exit(file_write_result);
    }

    /*
    for (size_t i = 0; i < file_line_count; i++) {
        printf("%s\n", (*decoded_output)[i]);
    }
    */

    /* free input file */
    for (size_t i = 0; i < file_line_count; i++) {
        free((*input_file)[i]);
    }
    free(*input_file);
    free(input_file);

    /* free decoded_output */
    for (size_t i = 0; i < file_line_count; i++) {
        free((*decoded_output)[i]);
    }
    free(*decoded_output);
    free(decoded_output);

    /* free blockers */
    list_free(blockers, &free_pt);
    free(blockers);

    /* free start & end pt */
    free(start_pt);
    free(end_pt);

    /* free return_path */
    list_free(return_path, &free_pt);
    free(return_path);

    /* free encoded_output */
    for (size_t i = 0; i < file_line_count; i++) {
        free((*encoded_output)[i]);
    }
    free(*encoded_output);
    free(encoded_output);

    printf("[Debug][main] main: program termindated successfully\n");
    exit(0);
}
