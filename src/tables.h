#ifndef TABLES_H
#define TABLES_H

typedef struct entry
{
    char *key;
    int *value;
} Entry;

typedef struct table
{
    Entry *entries;
    int capacity;
    int size;
} Table;

Table *create_table(int capacity);

void append(Table *table, char *key, int *value);

int *get_entry(Table *table, char *key);

void free_table(Table *table);

Table *init_symbol_table();

void print_table(Table *table);

#endif
