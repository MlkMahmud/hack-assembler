#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

int get_next_instruction(FILE *stream, char *buffer)
{
    int index = 0;
    int character;
    int has_reached_new_line = 0;

    while (!has_reached_new_line && (character = fgetc(stream)) != EOF)
    {
        if (character == '\n')
        {
            has_reached_new_line = 1;
        }
        else if (index < MAX_INSTRUCTION_SIZE - 1)
        {
            buffer[index] = (char)character;
            index++;
        }
    }

    buffer[index] = '\0';

    if (index == 0 && character == EOF)
    {
        return -1;
    }

    return 0;
}

bool is_a_instruction(char *buffer, Instruction *instr)
{
    const char *pattern = "^\\s*@(\\w+([.$]\\w+)*)(\\s\\/\\/\\s*[[:print:]]*\\s*)?$";
    regex_t re;
    regmatch_t match[2];

    if (regcomp(&re, pattern, REG_EXTENDED) != 0)
    {
        perror("failed to compile regex expression");
        exit(EXIT_FAILURE);
    }

    if (regexec(&re, buffer, 2, match, 0) != 0)
    {
        regfree(&re);
        return false;
    }

    size_t len = match[1].rm_eo - match[1].rm_so;
    instr->value = (char *)malloc(len + 1);

    if (instr->value == NULL)
    {
        regfree(&re);
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    instr->type = A_INSTRUCTION;
    strncpy(instr->value, buffer + match[1].rm_so, len);
    instr->value[len] = '\0';

    regfree(&re);
    return true;
}

bool is_comment_or_whitespace(char *buffer)
{
    const char *pattern = "(^\\s*$)|(^\\s*\\/\\/\\s*[[:print:]]*\\s*$)";
    regex_t re;
    regmatch_t match[2];

    if (regcomp(&re, pattern, REG_EXTENDED) != 0)
    {
        perror("failed to compile regex expression");
        exit(EXIT_FAILURE);
    }

    if (regexec(&re, buffer, 2, match, 0) != 0)
    {
        regfree(&re);
        return false;
    }

    regfree(&re);
    return true;
}

bool is_label_declaration(char *buffer, Instruction *instr)
{
    const char *pattern =
        "^\\s*\\(\\s*([a-zA-Z_][a-zA-Z0-9_]+([.$][a-zA-Z0-9_]+)*)\\s*\\)(\\s\\/\\/\\s*[[:print:]]*\\s*)?$";
    regex_t re;
    regmatch_t match[2];

    if (regcomp(&re, pattern, REG_EXTENDED) != 0)
    {
        perror("failed to compile regex expression");
        exit(EXIT_FAILURE);
    }

    if (regexec(&re, buffer, 2, match, 0) != 0)
    {
        regfree(&re);
        return false;
    }

    size_t len = match[1].rm_eo - match[1].rm_so;
    instr->label = (char *)malloc(len + 1);

    if (instr->label == NULL)
    {
        regfree(&re);
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    instr->type = LABEL_DECLARATION;
    strncpy(instr->label, buffer + match[1].rm_so, len);
    instr->label[len] = '\0';

    regfree(&re);
    return true;
}

void populate_symbol_table(FILE *stream, Table *table)
{
    char buffer[MAX_INSTRUCTION_SIZE];
    int status;
    int label_counter = 16;
    Instruction *instr = (Instruction *)malloc(sizeof(Instruction));

    while ((status = get_next_instruction(stream, buffer)) != -1)
    {
        if (is_a_instruction(buffer, instr))
        {
            printf("A INSTR: %s\n", instr->value);
            free(instr->value);
        }
        else if (is_label_declaration(buffer, instr))
        {
            // TOOD: Replace symbol values with real values.
            int symbol_value = label_counter + 1;

            append_symbol(table, instr->label, &symbol_value);
            label_counter++;

            free(instr->label);
        }
        else if (is_comment_or_whitespace(buffer))
        {
            continue;
        }

        buffer[0] = '\0';
    }

    free(instr);
    return;
};
