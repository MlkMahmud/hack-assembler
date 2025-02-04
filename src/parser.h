#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#include "tables.h"

#define MAX_INSTRUCTION_SIZE 128

int get_next_instruction(FILE *stream, char *buffer);

void populate_symbol_table(FILE *stream, Table *symbol_table);

#endif