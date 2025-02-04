#include <stdio.h>

#include "parser.h"

int get_next_instruction(FILE *stream, char *buffer)
{
    int index = 0;
    int character;
    int has_reached_new_line = 0;

    while (!has_reached_new_line && (character = fgetc(stream)) != EOF)
    {
        if (character == '\n')
        {
            has_reached_new_line = 1;
        }
        else if (index < MAX_INSTRUCTION_SIZE - 1)
        {
            buffer[index] = (char)character;
            index++;
        }
    }

    buffer[index] = '\0';

    if (index == 0 && character == EOF)
    {
        return -1;
    }

    return 0;
}

void populate_symbol_table(FILE *stream, Table *table)
{
    return;
};