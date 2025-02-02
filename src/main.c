#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tables.h"

bool is_asm_file(char* filename);

int main(int argc, char* argv[])
{
    if (argc != 2 || !is_asm_file(argv[1])) {
        fprintf(stderr,
            "usage: %s asm_file\n asm_file: an absolute or relative path to a .asm "
            "file.\n",
            argv[0]);
        return 1;
    }

    char* symbol_keys[] = {
        "R0",
        "R1",
        "R2",
        "R3",
        "R4",
        "R5",
        "R6",
        "R7",
        "R8",
        "R9",
        "R10",
        "R11",
        "R12",
        "R13",
        "R14",
        "R15",
        "SCREEN",
        "KBD",
        "SP",
        "LCL",
        "ARG",
        "THIS",
        "THAT"
    };

    int symbol_values[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16384, 24576, 0, 1, 2, 3, 4 };

    Table* symbol_table = create_table(sizeof(symbol_values) / sizeof(int));

    for (int index = 0; index < sizeof(symbol_values) / sizeof(int); index++) {
        append(symbol_table, symbol_keys[index], &symbol_values[index]);
    }

    print_table(symbol_table);
    free_table(symbol_table);
    return 0;
}

bool is_asm_file(char* filename)
{
    if (!filename)
        return false;

    char extension[] = ".asm";
    size_t extension_len = strlen(extension);
    size_t filenmame_len = strlen(filename);

    if (extension_len > filenmame_len)
        return false;

    return strcmp(filename + (filenmame_len - extension_len), extension) == 0;
}
