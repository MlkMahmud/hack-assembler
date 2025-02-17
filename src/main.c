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
    char *hack_filename;
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

    if ((hack_filename = generate_hack_filename(argv[1])) == NULL)
    {
        fprintf(stderr, "Failed to generate '.hack' file for src: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    };

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
    if (asm_filename == NULL)
    {
        return NULL;
    }

    if (!has_extension(asm_filename, ".asm"))
    {
        fprintf(stderr, "Filename '%s' does not end with '.asm'\n", asm_filename);
        exit(EXIT_FAILURE);
    }

    const char asm_ext[] = ".asm";
    const char hack_ext[] = ".hack";

    size_t asm_filename_len = strlen(asm_filename);
    size_t asm_ext_len = strlen(asm_ext);
    size_t hack_ext_len = strlen(hack_ext);

    char *hack_filename = safe_malloc((asm_filename_len - asm_ext_len) + (hack_ext_len + 1));

    strncpy(hack_filename, asm_filename, asm_filename_len - asm_ext_len);
    hack_filename[asm_filename_len - asm_ext_len] = '\0';
    strcat(hack_filename, hack_ext);

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
