#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <stdio.h>

#include "tables.h"

#define MAX_INSTRUCTION_SIZE 128

typedef struct
{
    char *comp;
    char *dest;
    char *jmp;
    char *label;
    char *value;
} Instruction;

int get_next_instruction(FILE *stream, char *buffer);

int parse_a_command(char *buffer, Instruction *instr, bool write_instr);

int parse_c_command(char *buffer, Instruction *instr, bool write_instr);

int parse_comment_or_whitespace(char *buffer);

int parse_label_declaration(char *buffer, Instruction *instr, bool write_instr);

#endif