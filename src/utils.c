#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void decimal_to_binary_str(unsigned int dec, char buffer[], size_t buffer_size, size_t word_size)
{
    int index;
    unsigned int max_value;

    if (buffer == NULL)
    {
        fprintf(stderr, "Error: NULL buffer\n");
        exit(EXIT_FAILURE);
    }

    if ((max_value = (1u << word_size) - 1) < dec)
    {
        fprintf(stderr, "Error: Decimal value %u is too large for %d bits.\n", dec, WORD_SIZE);
        exit(EXIT_FAILURE);
    }

    if (buffer_size != word_size + 1)
    {
        fprintf(stderr, "Error: Buffer size is invalid (expected %lu got %lu)\n", word_size + 1, buffer_size);
        exit(EXIT_FAILURE);
    }

    memset(buffer, '0', word_size);
    buffer[word_size] = '\0';
    index = word_size - 1;

    while (dec > 0 && index >= 0)
    {
        buffer[index] = (dec % 2) + '0';
        dec = dec / 2;
        index = index - 1;
    }

    return;
}

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
