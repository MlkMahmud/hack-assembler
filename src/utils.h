#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdlib.h>

#define WORD_SIZE 16

bool is_num_str(char *str);

void num_str_to_decimal(char *num_str, long *num);

void *safe_malloc(size_t size);

#endif
