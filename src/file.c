#include "../include/file.h"
#include "../include/errno.h"
#include <stdlib.h>
#include <string.h>

#define LMAX 255

void file_print_data(char **const data, const size_t line_count) {
    printf("file_print_data: data pointer = %p\n", data);
    printf("\nLines(%zu) in file:\n\n", line_count);
    for (size_t i = 0; i < line_count; i++) {
        char *line = (data)[i];
        printf("  array [%3zu] - len: %ld -  %s\n", i, strlen(line), line);
    }
}

int file_write(char **data, const size_t line_count, const char output_file_path[]) {
    printf("[Debug][file] file_write: writing to file - '%s'\n", output_file_path);

    FILE *fp = NULL;
    if (!(fp = fopen(output_file_path, "w"))) {
        fprintf(stderr, "[Error][file] file_write: failed to open file: '%s'\n", output_file_path);
        return ERROR_FILE_OPEN;
    }

    for (size_t i = 0; i < line_count; i++) {
        fprintf(fp, "%s\n", data[i]);
    }

    fclose(fp);

    return 0;
}

int file_read(char ***const data, size_t *const line_count, const char input_file_path[]) {
    printf("[Debug][file] file_read: reading from file - %s\n", input_file_path);

    char *ln = NULL;         /* NULL forces getline to allocate  */
    size_t n = 0;            /* buf size, 0 use getline default  */
    ssize_t line_length = 0; /* number of chars actually read    */
    size_t index = 0;        /* array index for number of lines  */
    size_t lmax = LMAX;      /* current array pointer allocation */
    FILE *fp = NULL;         /* file pointer                     */

    if (!(fp = fopen(input_file_path, "r"))) {
        fprintf(stderr, "[Error][file] file_read: failed to open file '%s'\n", input_file_path);
        return ERROR_FILE_OPEN;
    }

    char **line_array = malloc(LMAX * sizeof(char *));
    if (line_array == NULL) {
        fprintf(stderr, "[Error][file] file_read: unable to malloc line_array\n");
        return ERROR_MALLOC_FAILED;
    }

    while ((line_length = getline(&ln, &n, fp)) != -1) /* read line */
    {
        /* strip newline or carriage rtn */
        if (line_length > 0 && (ln[line_length - 1] == '\n' || ln[line_length - 1] == '\r'))
            ln[--line_length] = 0;

        if (line_length == 0) {
            continue;
        }

        line_array[index++] = strdup(ln);

        /* if lmax lines reached, realloc */
        if (index == lmax) {
            char **tmp = realloc(line_array, lmax * 2 * sizeof(char **));
            if (tmp == NULL) {
                fprintf(stderr, "[Error][file] file_read: unable to realloc line_array\n");
                return ERROR_MALLOC_FAILED;
            }

            line_array = tmp;
            lmax *= 2;
        }
    }

    *line_count = index;
    *data = line_array;

    /* Close file */
    if (fp != NULL) {
        fclose(fp);
    }

    /* free memory allocated to ln */
    if (ln != NULL) {
        free(ln);
    }

    return 0;
}
