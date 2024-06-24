#ifndef FILE_H_
#define FILE_H_

#include <stdio.h>
#include <stdlib.h>

void file_print_data(char **const data, const size_t line_count);
int file_write(char **const data, const size_t line_count, const char output_file_path[]);
int file_read(char ***const data, size_t *const line_count, const char input_file_path[]);

#endif
