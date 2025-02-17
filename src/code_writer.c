#include <stdio.h>

#include "code_writer.h"
#include "parser.h"
#include "utils.h"

void write_hack_commands(FILE *src_stream, FILE *out_stream, Table *symbol_table)
{
    int ram_addr = 16;
    char buffer[MAX_INSTRUCTION_SIZE];
    int line_number = 1;
    int status;

    Instruction *instr = safe_malloc(sizeof(Instruction));

    while ((status = get_next_instruction(src_stream, buffer)) != -1)
    {
        if (parse_a_command(buffer, instr) == 0)
        {
            if (is_num_str(instr->value))
            {
                long value = 0;
                num_str_to_decimal(instr->value, &value);

                if (value > HACK_MAX_INT) {
                    fprintf(stderr, "Error: The input value %ld exceeds the maximum allowed limit of %d. (Error at line %d)\n", value, HACK_MAX_INT, line_number);
                    exit(EXIT_FAILURE);
                }

                fprintf(out_stream, "%016lb\n", value);
                free(instr->value);
                continue;
            }

            int *symbol_value = get_entry(symbol_table, instr->value);

            if (symbol_value == NULL)
            {
                append_symbol(symbol_table, instr->value, ram_addr);
                ram_addr++;
            }

            symbol_value = get_entry(symbol_table, instr->value);
            fprintf(out_stream, "%016b\n", *symbol_value);
            free(instr->value);
        }
        else if (parse_c_command(buffer, instr) == 0)
        {
            if (instr->dest)
                free(instr->dest);

            if (instr->jmp)
                free(instr->jmp);

            free(instr->comp);

            continue;
        }
        else if (parse_label_declaration(buffer, instr) == 0)
        {
            free(instr->label);
            line_number++;
            continue;
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
}