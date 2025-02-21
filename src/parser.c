#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

int parse_a_command(char *buffer, Instruction *instr, bool write_instr)
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
        return 1;
    }

    regfree(&re);

    if (write_instr)
    {
        size_t len = match[1].rm_eo - match[1].rm_so;
    
        instr->value = (char *)safe_malloc(len + 1);
    
        strncpy(instr->value, buffer + match[1].rm_so, len);
        instr->value[len] = '\0';
    }

    return 0;
}

int parse_c_command(char *buffer, Instruction *instr, bool write_instr)
{
    const char *pattern =
        "^[[:space:]]*((M|D|MD|A|AM|AD|AMD)[[:space:]]*=[[:space:]]*)?(0|1|-1|[-!]?[ADM]|A[-+|&][1DM]|D[-+|&][1AM]|M[-+"
        "|&]["
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
        return 1;
    }

    regfree(&re);

    if (write_instr)
    {
        size_t comp_len = match[3].rm_eo - match[3].rm_so;
        size_t dest_len = match[2].rm_eo - match[2].rm_so;
        size_t jmp_len = match[5].rm_eo - match[5].rm_so;

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
    }

    return 0;
}

int parse_comment_or_whitespace(char *buffer)
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
        return 1;
    }

    regfree(&re);
    return 0;
}

int parse_label_declaration(char *buffer, Instruction *instr, bool write_instr)
{
    const char *pattern = "^[[:space:]]*\\([[:space:]]*([[:alpha:]_][[:alnum:]_]+([.$][[:alnum:]_]+)*)[[:space:]]*\\)(["
                          "[:space:]]+\\/\\/[[:print:]]*)?$";
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
        return 1;
    }

    regfree(&re);

    if (write_instr)
    {
        size_t len = match[1].rm_eo - match[1].rm_so;
    
        instr->label = (char *)safe_malloc(len + 1);
    
        strncpy(instr->label, buffer + match[1].rm_so, len);
        instr->label[len] = '\0';
    }


    return 0;
}
