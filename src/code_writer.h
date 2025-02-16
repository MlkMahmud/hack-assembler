#ifndef CODE_WRITER_H
#define CODE_WRITER_H

#include <stdio.h>

#include "tables.h"

void write_hack_commands(FILE *src_stream, FILE *out_stream, Table *symbol_table);

#endif