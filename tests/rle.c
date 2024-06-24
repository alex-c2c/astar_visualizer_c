#include "../include/rle.h"
#include "../include/file.h"
#include "./rle.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void test_rle3(char **str_array) {

    for (int i = 0; i < 10; i++) {
        str_array[i] = malloc(3 * sizeof(char));
        sprintf(str_array[i], "%02d", i);
    }

    const char *s = "test\0";
    printf("1 = %s, %ld, %ld\n", s, strlen(s), sizeof(s));

    char t[5] = "test\0";

    printf("2 = %s, %ld, %ld\n", t, strlen(t), sizeof(t));
}

void test_rle2() {
    char **str_array = malloc(10 * sizeof(char *));
    test_rle3(str_array);

    for (int i = 0; i < 10; i++) {
        printf("str = %s(%ld)\n", str_array[i], strlen((str_array)[i]));
    }

    free(str_array);
}

void test_rle() {
    /* this path CAN be relative to where the program is run! */
    // const char input_file_path[] = "./data/input_file_path.txt";
    // const char output_file_path[] = "./data/output_file_path.txt";

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

    /* read from file */
    char ***file_input = malloc(sizeof(char **));
    size_t file_line_count = 0;
    int8_t file_read_result = file_read(file_input, &file_line_count, input_file_path);
    if (file_read_result != 0) {
        fprintf(stderr, "[Error][rle] test_rle: unable to read from file\n");
        exit(-1);
    }

    /* test decoding */
    char ***decode_output = malloc(sizeof(char **));

    int decode_result = rle_decode(file_input, decode_output, file_line_count);
    if (decode_result != 0) {
        printf("[Debug][test_rle] test_rle: decoding failed\n");
        exit(decode_result);
    }

    for (size_t i = 0; i < file_line_count; i++) {
        printf("%s\n", (*decode_output)[i]);
    }

    /* test encoding */
    char ***encode_output = malloc(sizeof(char **));
    int encode_result = rle_encode(decode_output, encode_output, file_line_count);
    if (encode_result != 0) {
        printf("[Debug][test_rle] test_rle: encoding failed\n");
        exit(encode_result);
    }

    /*
    for (size_t i = 0; i < file_line_count; i++) {
        printf("%s\n", (*encode_output)[i]);
    }
    */

    int32_t file_write_result = file_write(*encode_output, file_line_count, output_file_path);
    if (file_write_result != 0) {
        printf("[Error][test_rle] test_rle: unable to write to file\n");
        exit(file_write_result);
    }

    /* free input lines (char*) */
    for (size_t i = 0; i < file_line_count; i++) {
        free((*file_input)[i]);
    }

    /* free the input_array holding the lines (char**) */
    free(*file_input);

    /* free the pointer holding the address of the input_array (char***) */
    free(file_input);

    /* free decode_output lines (char*) */
    for (size_t i = 0; i < file_line_count; i++) {
        free((*decode_output)[i]);
    }

    /* free the output_array holding the lines (char**) */
    free(*decode_output);

    /* free the pointer holding the address of the output_array (char***) */
    free(decode_output);

    /* free encode_output lines (char*) */
    for (size_t i = 0; i < file_line_count; i++) {
        free((*encode_output)[i]);
    }

    /* free the output_array holding the lines (char**) */
    free(*encode_output);

    /* free the pointer holding the address of the output_array (char***) */
    free(encode_output);

    /* testing how char array and string works */
    // char **test_output = malloc(sizeof(char *));
    // rle_test(test_output);
    // printf("test_output = %s\n", *test_output);

    exit(0);
}
