#ifndef FILE_H_
#define FILE_H_

#include <stdio.h>
#include <stdlib.h>

void file_print_data(char **const *const data, const size_t data_length);
int file_write(char **const *const data, const size_t data_length, const char output_file_path[]);
int file_read(char ***const data, size_t *const data_length, const char input_file_path[]);

#endif
