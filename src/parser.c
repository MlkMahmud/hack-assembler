#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "utils.h"

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
    const char *pattern = "^[[:space:]]*@([[:alnum:]_]+([.$][[:alnum:]_]+)*)([[:space:]]+\\/\\/[[:print:]]*)?$";
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
    size_t len = match[1].rm_eo - match[1].rm_so;

    instr->value = (char *)safe_malloc(len + 1);
    instr->type = A_INSTRUCTION;

    strncpy(instr->value, buffer + match[1].rm_so, len);
    instr->value[len] = '\0';

    return true;
}

bool is_c_instruction(char *buffer, Instruction *instr)
{
    const char *pattern = "^[[:space:]]*((M|D|MD|A|AM|AD|AMD)[[:space:]]*=[[:space:]]*)?(0|1|-1|[-!]?[ADM]|A[-+|&][1DM]|D[-+|&][1AM]|M[-+|&]["
                          "1DA])([[:space:]]*;[[:space:]]*(JLT|JEQ|JNE|JGT|JLE|JGE|JMP))?([[:space:]]+\\/\\/[[:print:]]*)?$";

    regex_t re;
    regmatch_t match[6];

    if (regcomp(&re, pattern, REG_EXTENDED) != 0)
    {
        perror("failed to compile regex expression");
        exit(EXIT_FAILURE);
    }

    if (regexec(&re, buffer, 6, match, 0) != 0)
    {
        regfree(&re);
        return false;
    }

    regfree(&re);

    size_t comp_len = match[3].rm_eo - match[3].rm_so;
    size_t dest_len = match[2].rm_eo - match[2].rm_so;
    size_t jmp_len = match[5].rm_eo - match[5].rm_so;

    instr->type = C_INSTRUCTION;
    instr->comp = (char *)safe_malloc(comp_len + 1);
    strncpy(instr->comp, buffer + match[3].rm_so, comp_len);
    instr->comp[comp_len] = '\0';
    instr->dest = NULL;
    instr->jmp = NULL;

    if (dest_len)
    {
        instr->dest = (char *)safe_malloc(dest_len + 1);
        strncpy(instr->dest, buffer + match[2].rm_so, dest_len);
        instr->dest[dest_len] = '\0';
    }

    if (jmp_len)
    {
        instr->jmp = (char *)safe_malloc(jmp_len + 1);
        strncpy(instr->jmp, buffer + match[5].rm_so, jmp_len);
        instr->jmp[jmp_len] = '\0';
    }

    return true;
}

bool is_comment_or_whitespace(char *buffer)
{
    const char *pattern = "(^[[:space:]]*$|^[[:space:]]*\\/\\/[[:print:]]*$)";
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
    const char *pattern = "^[[:space:]]*\\([[:space:]]*([[:alpha:]_][[:alnum:]_]+([.$][[:alnum:]_]+)*)[[:space:]]*\\)([[:space:]]+\\/\\/[[:print:]]*)?$";
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
    size_t len = match[1].rm_eo - match[1].rm_so;

    instr->label = (char *)safe_malloc(len + 1);
    instr->type = LABEL_DECLARATION;

    strncpy(instr->label, buffer + match[1].rm_so, len);
    instr->label[len] = '\0';

    return true;
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
        if (is_a_instruction(buffer, instr))
        {
            free(instr->value);
            current_instr_address++;
        }
        else if (is_c_instruction(buffer, instr))
        {
            if (instr->dest)
                free(instr->dest);

            if (instr->jmp)
                free(instr->jmp);

            free(instr->comp);
            current_instr_address++;
        }
        else if (is_label_declaration(buffer, instr))
        {

            append_symbol(table, instr->label, current_instr_address);
            free(instr->label);
        }
        else if (is_comment_or_whitespace(buffer))
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
