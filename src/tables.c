#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "tables.h"
#include "utils.h"

Table *create_table(int capacity)
{
    Table *table = (Table *)safe_malloc(sizeof(Table));

    table->entries = (Entry *)safe_malloc(capacity * sizeof(Entry));
    table->capacity = capacity;
    table->size = 0;

    return table;
}

void append_symbol(Table *table, char *key, int value)
{
    for (int index = 0; index < table->size; index++)
    {
        if (strcmp(table->entries[index].key, key) == 0)
        {
            *table->entries[index].value = value;
            return;
        }
    }

    if (table->size == table->capacity)
    {
        table->capacity = table->capacity * 2;
        table->entries = realloc(table->entries, sizeof(Entry) * table->capacity);

        if (table->entries == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    size_t key_len = strlen(key);
    table->entries[table->size].key = (char *)safe_malloc(key_len + 1);
    strncpy(table->entries[table->size].key, key, key_len);
    table->entries[table->size].key[key_len] = '\0';

    table->entries[table->size].value = (int *)safe_malloc(sizeof(int));
    *table->entries[table->size].value = value;
    table->size++;
    return;
}

int *get_entry(Table *table, char *key)
{
    for (int index = 0; index < table->size; index++)
        if (strcmp(table->entries[index].key, key) == 0)
            return table->entries[index].value;

    return NULL;
}

void free_table(Table *table)
{
    for (int index = 0; index < table->size; index++)
    {
        free(table->entries[index].key);
        free(table->entries[index].value);
    }

    free(table->entries);
    free(table);
}

Table *init_table(char *keys[], int values[], int initial_capacity)
{
    Table *table = create_table(initial_capacity);

    for (int i = 0; i < initial_capacity; i++)
    {
        append_symbol(table, keys[i], values[i]);
    }

    return table;
}

void populate_symbol_table(FILE *stream, Table *table)
{
    char buffer[MAX_INSTRUCTION_SIZE];
    int status;
    int current_instr_address = 0;
    int line_number = 1;
    Instruction *instr = safe_malloc(sizeof(Instruction));

    while ((status = get_next_instruction(stream, buffer)) != -1)
    {
        if (parse_a_command(buffer, instr, false) == 0 || parse_c_command(buffer, instr, false) == 0)
        {
            current_instr_address++;
        }
        else if (parse_label_declaration(buffer, instr, true) == 0)
        {
            append_symbol(table, instr->label, current_instr_address);
            free(instr->label);
        }
        else if (parse_comment_or_whitespace(buffer) == 0)
        {
            line_number++;
            continue;
        }
        else
        {
            free(instr);
            fprintf(stderr, "SyntaxError: invalid syntax (%s) on line %d\n", buffer, line_number);
            exit(EXIT_FAILURE);
        }
        line_number++;
        buffer[0] = '\0';
    }

    free(instr);
    return;
};

void print_table(Table *table)
{
    for (int index = 0; index < table->size; index++)
    {
        printf("[%s %d]\n", table->entries[index].key, *table->entries[index].value);
    }
}
