#include <stdio.h>

#include "code_writer.h"
#include "parser.h"
#include "utils.h"

void write_a_instruction(FILE *out_stream, Table *symbol_table, Instruction *instr, unsigned int *ram_addr)
{
    char hack_instruction[WORD_SIZE + 1];

    if (is_num_str(instr->value))
    {
        long value = 0;

        num_str_to_decimal(instr->value, &value);

        if (value > HACK_MAX_INT)
        {
            fprintf(stderr, "Error: The input value %ld exceeds the maximum allowed limit of %d.\n", value,
                    HACK_MAX_INT);
            exit(EXIT_FAILURE);
        }

        decimal_to_16_bit_binary_str(value, hack_instruction, sizeof(hack_instruction), WORD_SIZE);
        fprintf(out_stream, "%s\n", hack_instruction);
        free(instr->value);
        return;
    }

    int *symbol_value = get_entry(symbol_table, instr->value);

    if (symbol_value == NULL)
    {
        append_symbol(symbol_table, instr->value, *ram_addr);
        *ram_addr += 1;
    }

    symbol_value = get_entry(symbol_table, instr->value);
    decimal_to_16_bit_binary_str(*symbol_value, hack_instruction, sizeof(hack_instruction), WORD_SIZE);
    fprintf(out_stream, "%s\n", hack_instruction);
    free(instr->value);
    return;
}

void write_hack_commands(FILE *src_stream, FILE *out_stream, Table *symbol_table)
{
    unsigned int ram_addr = 16;
    char buffer[MAX_INSTRUCTION_SIZE];
    int line_number = 1;
    int status;

    Instruction *instr = safe_malloc(sizeof(Instruction));

    while ((status = get_next_instruction(src_stream, buffer)) != -1)
    {
        if (parse_a_command(buffer, instr, true) == 0)
        {
            write_a_instruction(out_stream, symbol_table, instr, &ram_addr);
        }
        else if (parse_c_command(buffer, instr, true) == 0)
        {
            if (instr->dest)
                free(instr->dest);

            if (instr->jmp)
                free(instr->jmp);

            free(instr->comp);
        }
        else if (parse_label_declaration(buffer, instr, false) == 0 || parse_comment_or_whitespace(buffer) == 0)
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