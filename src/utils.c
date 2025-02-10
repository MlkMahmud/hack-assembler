#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

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
