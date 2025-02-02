#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tables.h"

Table *create_table(int capacity) {
  Table *table = (Table *)malloc(sizeof(Table));

  if (table == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  table->entries = (Entry *)malloc(capacity * sizeof(Entry));

  if (table->entries == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  table->capacity = capacity;
  table->size = 0;

  return table;
}

void append(Table *table, char *key, int *value) {
  int *value_cpy = malloc(sizeof(int));

  if (value_cpy == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  for (int index = 0; index < table->size; index++) {
    if (strcmp(table->entries[index].key, key) == 0) {
      *value_cpy = *value;
      table->entries[index].value = value_cpy;
      return;
    }
  }

  if (table->size == table->capacity) {
    table->capacity = table->capacity * 2;
    table->entries = realloc(table->entries, sizeof(Entry) * table->capacity);

    if (table->entries == NULL) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(EXIT_FAILURE);
    }
  }

  table->entries[table->size].key = strdup(key);

  if (table->entries[table->size].key == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  *value_cpy = *value;
  table->entries[table->size].value = value_cpy;
  table->size++;
  return;
}

int *get_entry(Table *table, char *key) {
  for (int index = 0; index < table->size; index++)
    if (strcmp(table->entries[index].key, key) == 0)
      return table->entries[index].value;

  return NULL;
}

void free_table(Table *table) {
  for (int index = 0; index < table->size; index++) {
    free(table->entries[index].key);
    free(table->entries[index].value);
  }

  free(table->entries);
  free(table);
}

Table *init_symbol_table() {
  char *keys[] = {"R0",     "R1",  "R2",  "R3",  "R4",  "R5",   "R6",  "R7",
                  "R8",     "R9",  "R10", "R11", "R12", "R13",  "R14", "R15",
                  "SCREEN", "KBD", "SP",  "LCL", "ARG", "THIS", "THAT"};

  int values[] = {0,  1,  2,  3,  4,     5,     6, 7, 8, 9, 10, 11,
                  12, 13, 14, 15, 16384, 24576, 0, 1, 2, 3, 4};

  int table_size = sizeof(values) / sizeof(int);

  Table *table = create_table(table_size);

  for (int i = 0; i < table_size; i++) {
    append(table, keys[i], &values[i]);
  }

  return table;
}

void print_table(Table *table) {
  for (int index = 0; index < table->size; index++) {
    printf("[%s %d]\n", table->entries[index].key,
           *table->entries[index].value);
  }
}
