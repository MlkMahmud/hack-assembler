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

Table *init_comp_table() { 
    Table *comp_table = safe_malloc(sizeof(Table));
    
    char entries[][2][7] = {
        {"0", "0101010"}, {"1", "0111111"}, {"-1", "0111010"}, {"D", "0001100"},
        {"A", "0110000"}, {"M", "1110000"}, {"!D", "0001101"}, {"!A", "0110001"},
        {"!M", "1110001"}, {"-D", "0001111"}, {"-A", "0110011"}, {"-M", "1110011"},
        {"D+1", "0011111"}, {"A+1", "0110111"}, {"M+1", "1110111"}, {"D-1", "0001110"},
        {"A-1", "0110010"}, {"M-1", "1110010"}, {"D+A", "0000010"}, {"A+D", "0000010"},
        {"D+M", "1000010"}, {"M+D", "1000010"}, {"D-A", "0010011"}, {"D-M", "1010011"},
        {"A-D", "0000111"}, {"M-D", "1000111"}, {"D&A", "0000000"}, {"A&D", "0000000"},
        {"D&M", "1000000"}, {"M&D", "1000000"}, {"D|A", "0010101"}, {"A|D", "0010101"},
        {"D|M", "1010101"}, {"M|D", "1010101"}
    };

    printf("Table: %s\n", entries[0][0]);
    return comp_table;
    
}

Table *init_symbol_table()
{
    char *keys[] = {"R0",  "R1",  "R2",  "R3",  "R4",     "R5",  "R6", "R7",  "R8",  "R9",   "R10", "R11",
                    "R12", "R13", "R14", "R15", "SCREEN", "KBD", "SP", "LCL", "ARG", "THIS", "THAT"};

    int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16384, 24576, 0, 1, 2, 3, 4};

    int capacity = sizeof(values) / sizeof(int);

    Table *table = create_table(capacity);

    for (int i = 0; i < capacity; i++)
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
    Instruction *instr = (Instruction *)safe_malloc(sizeof(Instruction));

    while ((status = get_next_instruction(stream, buffer)) != -1)
    {
        if (parse_a_command(buffer, instr) == 0)
        {
            free(instr->value);
            current_instr_address++;
        }
        else if (parse_c_command(buffer, instr) == 0)
        {
            if (instr->dest)
                free(instr->dest);

            if (instr->jmp)
                free(instr->jmp);

            free(instr->comp);
            current_instr_address++;
        }
        else if (parse_label_declaration(buffer, instr) == 0)
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
