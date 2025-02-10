#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <stdio.h>

#include "tables.h"

#define MAX_INSTRUCTION_SIZE 128

typedef enum
{
    A_INSTRUCTION,
    C_INSTRUCTION,
    LABEL_DECLARATION,
} InstructionType;

typedef struct
{
    InstructionType type;
    char *comp;
    char *dest;
    char *jmp;
    char *label;
    char *value;
} Instruction;

int get_next_instruction(FILE *stream, char *buffer);

bool is_a_instruction(char *buffer, Instruction *instr);

bool is_c_instruction(char *buffer, Instruction *instr);

bool is_comment_or_whitespace(char *buffer);

bool is_label_declaration(char *buffer, Instruction *instr);

void populate_symbol_table(FILE *stream, Table *symbol_table);

#endif