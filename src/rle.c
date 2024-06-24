#include "../include/rle.h"
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint8_t get_digit_count(size_t n) {
    if (n < 0)
        return get_digit_count((n == INT_MAX) ? INT_MAX : -n);
    if (n < 10)
        return 1;
    return 1 + get_digit_count(n / 10);
}

void strcat_c(char *str, char c) {
    for (; *str; str++)
        ; // note the terminating semicolon here.
    *str++ = c;
}
/*
 * Input should always be a CHAR follow by DIGITS (of any size)
 * E.g.
 * Good - A1B2C3D4A11B200C3000
 * Bad - AB200CD100DEF200
 * */
int rle_decode_line(char **input_line, char **output_line) {
    printf("[Debug][rle] rle_decode_line: %s (len: %ld)\n", *input_line, strlen(*input_line));
    uint32_t output_length = 0;
    uint32_t char_count = 0;
    uint32_t *count_array = NULL;
    char *char_array = NULL;

    for (uint32_t i = 0; i < strlen(*input_line); i++) {
        if (isdigit((*input_line)[i]) == 0) {
            char_count++;
        }
    }

    printf("[Debug][rle] rle_decode_line: char_count = %d\n", char_count);
    count_array = malloc(char_count * sizeof(uint32_t));
    char_array = malloc((char_count + 1) * sizeof(char));
    char_array[char_count] = '\0';

    uint32_t tmp_index = 0;
    uint32_t digit_start_index = -1;

    for (int i = strlen(*input_line) - 1; i >= 0; i--) {
        char c = (*input_line)[i];
        if (isdigit(c) > 0) {
            if (digit_start_index == -1) {
                digit_start_index = i;
            }
        } else {
            uint32_t digit_count = digit_start_index - i;
            if (digit_start_index != -1 && digit_count > 0) {
                char *num_string = malloc((digit_count + 1) * sizeof(char));
                strncpy(num_string, *input_line + i + 1, digit_count);

                int num = atoi(num_string);
                output_length += num;

                char_array[tmp_index] = c;
                count_array[tmp_index] = num;
                tmp_index++;

                free(num_string);
            }
        }
    }

    printf("[Debug][rle] rle_decode_line: output_length = %d\n", output_length);
    *output_line = malloc((output_length + 1) * sizeof(char));

    uint32_t output_index = 0;
    /* side note: using unsigned i with a reverse for loop + >= 0 limiter, will make it enter negative region */
    for (int32_t i = char_count - 1; i >= 0; i--) {
        char c = char_array[i];
        // printf("adding: '%c', %d times\n", c, count_array[i]);
        for (uint32_t j = 0; j < count_array[i]; j++) {
            /* i don't know why using (*output_line)[++output_index] = c doesn't work at all */
            (*output_line)[output_index] = c;
            output_index++;
        }
    }

    /* remember to teminated the string */
    (*output_line)[output_length] = '\0';

    free(count_array);
    free(char_array);

    return 0;
}

int rle_encode_line(char **input_line, char **output_line) {
    printf("[Debug][rle] rle_encode_line: input_line len = %ld\n", strlen(*input_line));
    uint32_t line_len = strlen(*input_line);

    uint32_t char_count = 0;
    char curr_c = '\0';
    for (uint32_t i = 0; i < line_len; i++) {
        if (curr_c != (*input_line)[i]) {
            char_count++;
            curr_c = (*input_line)[i];
        }
    }

    printf("[Debug][rle] rle_encode_line: char_count = %d\n", char_count);
    char *char_array = malloc(char_count * sizeof(char));
    uint32_t *count_array = malloc(char_count * sizeof(uint32_t));

    for (uint32_t i = 0; i < char_count; i++) {
        char_array[i] = '\0';
        count_array[i] = 0;
    }

    uint32_t char_index = -1;
    for (uint32_t i = 0; i < line_len; i++) {
        if (char_index == -1 || (*input_line)[i] != char_array[char_index]) {
            char_index++;
            char_array[char_index] = (*input_line)[i];
        }

        count_array[char_index]++;
    }

    uint32_t output_len = char_count;
    uint32_t *digit_array = malloc(char_count * sizeof(uint32_t));
    for (uint32_t i = 0; i < char_count; i++) {
        uint32_t digit_count = get_digit_count(count_array[i]);
        digit_array[i] = digit_count;
        output_len += digit_count;
    }

    printf("[Debug][rle] rle_encode_line: ");
    for (uint32_t i = 0; i < char_count; i++) {
        printf("%c,%d,%d ", char_array[i], count_array[i], digit_array[i]);
    }
    printf("\n");

    uint32_t tmp_index = 0;
    *output_line = malloc((output_len + 1) * sizeof(char));
    for (uint32_t i = 0; i < char_count; i++) {
        (*output_line)[tmp_index] = char_array[i];
        tmp_index++;

        char *digit_str = malloc(digit_array[i] * sizeof(char));
        sprintf(digit_str, "%d", count_array[i]);

        for (uint32_t j = 0; j < digit_array[i]; j++) {
            (*output_line)[tmp_index] = digit_str[j];
            tmp_index++;
        }

        free(digit_str);
    }

    (*output_line)[tmp_index] = '\0';

    free(char_array);
    free(count_array);
    free(digit_array);

    return 0;
}

/*
 * *output & (*output)[i] needs to be freed by the method calling this
 */
int rle_encode(char ***input, char ***output, const size_t line_count) {
    char **output_array = malloc(line_count * sizeof(char *));

    for (size_t i = 0; i < line_count; i++) {
        char *input_line = (*input)[i];
        char *output_line = malloc(sizeof(char *));

        int decode_result = rle_encode_line(&input_line, &output_line);
        if (decode_result != 0) {
            printf("[Debug][rle] rle_encode: unable to decode line(idx = %ld) = %s\n", i, input_line);
            return decode_result;
        }

        output_array[i] = output_line;
    }

    *output = output_array;

    return 0;
}

/*
 * *output & (*output)[i] needs to be freed by the method calling this
 */
int rle_decode(char **const *const input, char ***const output, const size_t line_count) {
    char **output_array = malloc(line_count * sizeof(char *));

    for (size_t i = 0; i < line_count; i++) {
        char *input_line = (*input)[i];
        char *output_line = malloc(sizeof(char *));

        int decode_result = rle_decode_line(&input_line, &output_line);
        if (decode_result != 0) {
            printf("[Debug][rle] rle_decode: unable to decode line(idx = %ld) = %s\n", i, input_line);
            return decode_result;
        }

        output_array[i] = output_line;
    }

    *output = output_array;

    return 0;
}

/*
int rle_test(char **output_line) {
    printf("\n");
    //(*output_line) = malloc(9 * sizeof(char *));
    char array1[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    array1[9] = (char)0;
    printf("array1 = %s\n", array1);
    printf("array1 len = %ld\n", strlen(array1));
    printf("array1 size = %ld\n", sizeof(*array1));
    printf("array1 = %p\n", &array1);
    // printf("array1 = %c\n", array1[10]);
    // printf("array1 = %c\n", array1[11]);
    // printf("array1 = %c\n", array1[12]);
    printf("\n");

    char test[] = {'t', 'e', 's', 't', '\0'};
    char *array2 = malloc(13 * sizeof(char));
    array2[0] = test[0];
    array2[1] = test[1];
    array2[2] = test[2];
    array2[3] = test[3];
    array2[4] = test[0];
    array2[5] = test[1];
    array2[6] = test[2];
    array2[7] = test[3];
    array2[8] = test[0];
    array2[9] = test[1];
    array2[10] = test[2];
    array2[11] = test[3];
    array2[12] = '\0';

    printf("array2 = %s\n", array2);
    printf("array2 len = %ld\n", strlen(array2));
    printf("array2 size = %ld\n", sizeof(*array2));
    printf("array2 = %p\n", &array2);
    printf("\n");

    *output_line = array2;

    // char array1[5] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    char *string = "0123456789";

    printf("string = %s\n", string);
    printf("string len = %ld\n", strlen(string));
    printf("string size = %ld\n", sizeof(*string));
    printf("string pt = %p\n", &string);
    printf("\n");

    char array3[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    printf("array3 = %s\n", array3);
    printf("array3 len = %ld\n", strlen(array3));
    printf("array3 size = %ld\n", sizeof(*array3));
    printf("array3 = %p\n", &array3);
    printf("\n");

    char array4[10] = "01234567890123456789";

    printf("array4 = %s\n", array4);
    printf("array4 len = %ld\n", strlen(array4));
    printf("array4 size = %ld\n", sizeof(*array4));
    printf("array4 = %p\n", &array4);
    printf("\n");

    char array5[] = "01234567890123456789";

    printf("array5 = %s\n", array5);
    printf("array5 len = %ld\n", strlen(array5));
    printf("array5 size = %ld\n", sizeof(*array5));
    printf("array5 = %p\n", &array5);
    printf("\n");

    return 0;
}
*/
