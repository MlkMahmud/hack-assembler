#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code_writer.h"
#include "parser.h"
#include "tables.h"
#include "utils.h"

char *generate_hack_filename(char *asm_filename);

bool has_extension(char *filename, char *extension);

int main(int argc, char *argv[])
{
    FILE *src_stream;
    FILE *out_stream;
    Table *symbol_table;

    if (argc != 2 || !has_extension(argv[1], ".asm"))
    {
        fprintf(stderr,
                "usage: %s asm_file\n asm_file: an absolute or relative path to a .asm "
                "file.\n",
                argv[0]);
        return 1;
    }

    if ((src_stream = fopen(argv[1], "r")) == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    symbol_table = init_symbol_table();
    populate_symbol_table(src_stream, symbol_table);

    char *hack_filename = generate_hack_filename(argv[1]);

    if ((out_stream = fopen(hack_filename, "w")) == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(src_stream, 0, SEEK_SET);
    write_hack_commands(src_stream, out_stream, symbol_table);
    fclose(src_stream);
    fclose(out_stream);
    free(hack_filename);
    free_table(symbol_table);

    return 0;
}

char *generate_hack_filename(char *asm_filename)
{
    const int ASM_EXT_LEN = strlen(".asm");
    const char HACK_FILE_EXT[] = ".hack";

    size_t asm_filename_len = strlen(asm_filename);
    //  The ".hack" extension contains four characters while the ".asm" extension has three.
    //  We allocate one (1) extra byte to account for this difference, and another extra byte for the NULL terminator.
    char *hack_filename = safe_malloc(asm_filename_len + sizeof(char) + 1);

    // copy everything except the last (4) bytes ".asm"
    strncpy(hack_filename, asm_filename, asm_filename_len - ASM_EXT_LEN);
    hack_filename[asm_filename_len - ASM_EXT_LEN] = '\0';
    strcat(hack_filename, HACK_FILE_EXT);

    return hack_filename;
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
