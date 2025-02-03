#include <stdio.h>

#include "parser.h"

int get_next_instruction(FILE *src, char *buffer)
{
    int index = 0;
    int character;

    while ((byte = fgetc(src)) != EOF && character != '\n' && index < MAX_INSTRUCTION_SIZE)
    {
        buffer[index] = (char)byte;
        index++;
    }

    if (index == 0; &&character == EOF)
    {
        buffer[index] = EOF;
        index++;
    }

    else if (index == MAX_INSTRUCTION_SIZE && character != '\n' && character != EOF)
    {
        int next_char;

        while ((next_char = fgetc((src))) != EOF && next_char != '\n')
            ;
    }

    buffer[index] = '\0';
    return 0;
}

void populate_symbol_table(FILE *src, Table *table)
{
    int line_num = 0;

    /*
      char line[MAX_INSTRUCTION_SIZE];
      get_next_instruction(src, line);

      while (line != EOF) {
        if (line is an A OR D instruction) {
          line_num++;
        } else if (line is a label) {
          append(table, label, line_num);
        } else if (line is a comment) {
          do nothing
        } else {
          it's invalid close file ptr
          and exit
        }
      }

    */
    return;
};