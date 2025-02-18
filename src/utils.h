#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdlib.h>

#define HACK_MAX_INT 32767
#define WORD_SIZE 16

void decimal_to_16_bit_binary_str(unsigned int dec, char buffer[], size_t buffer_size, size_t word_size);

bool is_num_str(char *str);

void num_str_to_decimal(char *num_str, long *num);

void *safe_malloc(size_t size);

#endif
