#include <stdio.h>

#include "tables.h"

#ifndef PARSER_H
#define PARSER_H

const int MAX_INSTRUCTION_SIZE = 128;

int get_next_instruction(char *buffer);

void populate_symbol_table(FILE *src, Table *symbol_table);

#endif