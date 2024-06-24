#ifndef RLE_H_
#define RLE_H_

#include <stdio.h>
#include <stdlib.h>

int rle_encode(char ***input, char ***output, const size_t line_count);
int rle_decode(char **const *const input, char ***const output, const size_t line_count);
// int rle_decode_line(char **input_line, char **output_line);
// int rle_test(char **output_line);

#endif
