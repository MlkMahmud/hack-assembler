#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <stdio.h>

#include "tables.h"

#define MAX_INSTRUCTION_SIZE 128

typedef enum
{
    A_INSTRUCTION,
    D_INSTRUCTION,
    LABEL_DECLARATION,
} InstructionType;

typedef struct
{
    InstructionType type;
    char *label;
} Instruction;

int get_next_instruction(FILE *stream, char *buffer);

bool is_label_declaration(char *buffer, Instruction *instruction);

void populate_symbol_table(FILE *stream, Table *symbol_table);

#endif