#ifndef CODE_WRITER_H
#define CODE_WRITER_H

#include <stdio.h>

#include "parser.h"
#include "tables.h"

void write_a_instruction(FILE *out_stream, Table *symbol_table, Instruction *instr, unsigned int *ram_address);

void write_hack_commands(FILE *src_stream, FILE *out_stream, Table *symbol_table);

#endif