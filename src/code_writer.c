#include <stdio.h>
#include <string.h>

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

        decimal_to_binary_str(value, hack_instruction, sizeof(hack_instruction), WORD_SIZE);
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
    decimal_to_binary_str(*symbol_value, hack_instruction, sizeof(hack_instruction), WORD_SIZE);
    fprintf(out_stream, "%s\n", hack_instruction);
    free(instr->value);
    return;
}

void write_c_instruction(FILE *out_stream, Instruction *instr, Table *comp_table, Table *dest_table, Table *jmp_table)
{
    char hack_instruction[WORD_SIZE + 1];
    char comp_bits[8];
    char dest_bits[] = "000";
    char jump_bits[] = "000";
    char leading_bits[] = "111";
    size_t leading_bits_len = sizeof(leading_bits) / sizeof(char);

    int *comp_value = get_entry(comp_table, instr->comp);
    decimal_to_binary_str(*comp_value, comp_bits, sizeof(comp_bits), sizeof(comp_bits) - 1);
    free(instr->comp);

    if (instr->dest)
    {
        int *dest_value = get_entry(dest_table, instr->dest);
        decimal_to_binary_str(*dest_value, dest_bits, sizeof(dest_bits), sizeof(dest_bits) - 1);
        free(instr->dest);
    }

    if (instr->jmp)
    {
        int *jmp_value = get_entry(jmp_table, instr->jmp);
        decimal_to_binary_str(*jmp_value, jump_bits, sizeof(jump_bits), sizeof(jump_bits) - 1);
        free(instr->jmp);
    }

    strncpy(hack_instruction, leading_bits, leading_bits_len);
    hack_instruction[leading_bits_len] = '\0';

    strcat(hack_instruction, comp_bits);
    strcat(hack_instruction, dest_bits);
    strcat(hack_instruction, jump_bits);

    fprintf(out_stream, "%s\n", hack_instruction);
    return;
}

void write_hack_instructions(FILE *src_stream, FILE *out_stream, Table *symbol_table)
{
    int status;
    char buffer[MAX_INSTRUCTION_SIZE];

    Table *comp_table =
        init_table((char *[34]){"0",   "1",   "-1",  "D",   "A",   "M",   "!D",  "!A",  "!M",  "-D",  "-A",  "-M",
                                "D+1", "A+1", "M+1", "D-1", "A-1", "M-1", "D+A", "A+D", "D+M", "M+D", "D-A", "D-M",
                                "A-D", "M-D", "D&A", "A&D", "D&M", "M&D", "D|A", "A|D", "D|M", "M|D"},
                   (int[34]){42,  63, 58, 12, 48, 112, 13, 49, 113, 15, 51, 115, 31, 55, 119, 14, 50,
                             114, 2,  2,  66, 66, 19,  83, 7,  71,  0,  0,  64,  64, 21, 21,  85, 85},
                   (34));

    Table *dest_table = init_table(
        (char *[15]){"M", "D", "DM", "MD", "A", "AM", "MA", "AD", "DA", "ADM", "AMD", "MDA", "MAD", "DAM", "DMA"},
        (int[15]){1, 2, 3, 3, 4, 5, 5, 6, 6, 7, 7, 7, 7, 7, 7}, 15);

    Table *jmp_table =
        init_table((char *[7]){"JGT", "JEQ", "JGE", "JLT", "JNE", "JLE", "JMP"}, (int[]){1, 2, 3, 4, 5, 6, 7}, 7);

    Instruction *instr = safe_malloc(sizeof(Instruction));
    int line_number = 1;
    unsigned int ram_addr = 16;

    while ((status = get_next_instruction(src_stream, buffer)) != -1)
    {
        if (parse_a_command(buffer, instr, true) == 0)
        {
            write_a_instruction(out_stream, symbol_table, instr, &ram_addr);
        }
        else if (parse_c_command(buffer, instr, true) == 0)
        {
            write_c_instruction(out_stream, instr, comp_table, dest_table, jmp_table);
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

    free_table(comp_table);
    free_table(dest_table);
    free_table(jmp_table);
    free(instr);
    return;
}