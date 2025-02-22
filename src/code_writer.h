#ifndef CODE_WRITER_H
#define CODE_WRITER_H

#include <stdio.h>

#include "parser.h"
#include "tables.h"

void write_a_instruction(FILE *out_stream, Table *symbol_table, Instruction *instr, unsigned int *ram_address);

void write_c_instruction(FILE *outstream, Instruction *instr, Table *comp_table, Table *dest_table, Table *jmp_table);

void write_hack_instructions(FILE *src_stream, FILE *out_stream, Table *symbol_table);

#endif