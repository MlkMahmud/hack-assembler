#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tables.h"

bool is_asm_file(char *filename);

int main(int argc, char *argv[]) {
  if (argc != 2 || !is_asm_file(argv[1])) {
    fprintf(
        stderr,
        "usage: %s asm_file\n asm_file: an absolute or relative path to a .asm "
        "file.\n",
        argv[0]);
    return 1;
  }

  Table *symbol_table = init_symbol_table();
  print_table(symbol_table);
  free_table(symbol_table);
  return 0;
}

bool is_asm_file(char *filename) {
  if (!filename)
    return false;

  char extension[] = ".asm";
  size_t extension_len = strlen(extension);
  size_t filenmame_len = strlen(filename);

  if (extension_len > filenmame_len)
    return false;

  return strcmp(filename + (filenmame_len - extension_len), extension) == 0;
}
