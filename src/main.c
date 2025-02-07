#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "tables.h"

bool has_extension(char *filename, char *extension);

int main(int argc, char *argv[])
{
    FILE *stream;
    Table *symbol_table;

    if (argc != 2 || !has_extension(argv[1], ".asm"))
    {
        fprintf(stderr,
                "usage: %s asm_file\n asm_file: an absolute or relative path to a .asm "
                "file.\n",
                argv[0]);
        return 1;
    }

    if ((stream = fopen(argv[1], "r")) == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    symbol_table = init_symbol_table();
    populate_symbol_table(stream, symbol_table);

    print_table(symbol_table);
    fclose(stream);
    free_table(symbol_table);

    return 0;
}

bool has_extension(char *filename, char *extension)
{
    if (!filename)
        return false;

    size_t extension_len = strlen(extension);
    size_t filenmame_len = strlen(filename);

    if (extension_len > filenmame_len)
        return false;

    return strcmp(filename + (filenmame_len - extension_len), extension) == 0;
}
