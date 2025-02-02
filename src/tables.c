#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tables.h"

Table* create_table(int capacity)
{
    Table* table = (Table*)malloc(sizeof(Table));

    if (table == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    table->entries = (Entry*)malloc(capacity * sizeof(Entry));

    if (table->entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    table->capacity = capacity;
    table->size = 0;

    return table;
}

void append(Table* table, char* key, int* value)
{
    for (int index = 0; index < table->size; index++) {
        if (strcmp(table->entries[index].key, key) == 0) {
            table->entries[index].value = value;
            return;
        }
    }

    // TODO: add functionality to resize table capacity.
    if (table->size == table->capacity) {
        fprintf(stderr, "Table is at maximum capacity\n");
        exit(EXIT_FAILURE);
    }

    table->entries[table->size].key = strdup(key);

    if (table->entries[table->size].key == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    table->entries[table->size].value = value;
    table->size++;
    return;
}

int* get_entry(Table* table, char* key)
{
    for (int index = 0; index < table->size; index++)
        if (strcmp(table->entries[index].key, key) == 0)
            return table->entries[index].value;

    return NULL;
}

void free_table(Table* table)
{
    for (int index = 0; index < table->size; index++)
        free(table->entries[index].key);

    free(table->entries);
    free(table);
}

void print_table(Table* table)
{
    for (int index = 0; index < table->size; index++) {
        printf("[%s %d]\n", table->entries[index].key, *table->entries[index].value);
    }
}