#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

bool is_num_str(char *str)
{
    int len = strlen(str);

    for (int i = 0; i < len; i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }

    return true;
}

void num_str_to_decimal(char *num_str, long *num)
{
    char *endptr;
    errno = 0;

    *num = strtol(num_str, &endptr, 10);

    if (errno == ERANGE)
    {
        perror("strtol");
        exit(EXIT_FAILURE);
    }

    if (endptr == num_str)
    {
        fprintf(stderr, "No digits were found\n");
        exit(EXIT_FAILURE);
    }

    if (*endptr != '\0')
    {
        fprintf(stderr, "Further characters after number: \"%s\"\n", endptr);
        exit(EXIT_FAILURE);
    }
}

void print_dec_as_binary(FILE *stream, long num, size_t word_size)
{
    for (int i = word_size - 1; i >= 0; i--)
    {
        fprintf(stream, "%ld", (num >> i) & 1);
    }

    fprintf(stream, "\n");
}

void *safe_malloc(size_t size)
{
    void *addr = malloc(size);

    if (addr == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    return addr;
}
