#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char hexadecimalnum[5], object_code[10], opcode_value[5], operand_address[5];
char opcode[10], label[10], operand[10], prg_len[10], address[10], start_address[5];
int locctr, no_of_objcodes, n;
FILE *inter, *length, *objcode, *counts;

void clear(char *arr)
{
    int len = strlen(arr);
    for (int i = 0; i < len; i++)
        arr[i] = '\000';
}

char *hex(int decimalnum)
{
    clear(hexadecimalnum);
    int quotient, remainder;
    char temp;
    quotient = decimalnum;
    int i = -1;
    while (quotient > 0)
    {
        remainder = quotient % 16;
        if (remainder < 10)
            hexadecimalnum[++i] = '0' + remainder;
        else
            hexadecimalnum[++i] = 'A' + (remainder - 10);
        quotient = quotient / 16;
    }
    for (int j = 0; j <= i / 2; j++)
    {
        temp = hexadecimalnum[j];
        hexadecimalnum[j] = hexadecimalnum[i - j];
        hexadecimalnum[i - j] = temp;
    }
    return hexadecimalnum;
}

void get_value_from_optab(char *op)
{
    FILE *optab = fopen("optab.txt", "r");
    char opcode[10];
    while (!feof(optab))
    {
        fscanf(optab, "%s %s\n", opcode, opcode_value);
        if (strcmp(op, opcode) == 0)
        {
            fclose(optab);
            return;
        }
    }
    clear(opcode_value);
}

void get_value_from_symtab(char *sym)
{
    FILE *symtab = fopen("symtab.txt", "r");
    char label[10];
    while (!feof(symtab))
    {
        fscanf(symtab, "%s %s\n", label, operand_address);
        if (strcmp(sym, label) == 0)
        {
            fclose(symtab);
            return;
        }
    }
    clear(operand_address);
}

void write_value_as_n_bits_to_the_file(char *value, int n, FILE *file, char filler)
{
    int len = strlen(value);
    if (filler == '0')
        while (len < n)
        {
            fprintf(file, "0");
            n--;
        }
    fprintf(file, "%s", value);
    if (filler != '0')
        while (len < n)
        {
            fprintf(file, "%c", filler);
            n--;
        }
    return;
}

void put_a_seperator(FILE *file)
{
    fprintf(file, "^");
}

void put_a_newline(FILE *file)
{
    fprintf(file, "\n");
}

void generate_object_code()
{
    clear(object_code);
    strcat(object_code, opcode_value);
    strcat(object_code, operand_address);
}

int no_of_objcodes_to_be_created()
{
    if (!feof(counts))
    {
        if (no_of_objcodes >= 10)
        {
            no_of_objcodes -= 10;
            return 10;
        }
        else if (no_of_objcodes > 0)
        {
            int t = no_of_objcodes;
            no_of_objcodes = 0;
            return t;
        }
        else if (no_of_objcodes == 0)
        {
            fscanf(inter, "%s %s %s %s\n", address, label, opcode, operand);
            fscanf(counts, "%i", &no_of_objcodes);
            if (no_of_objcodes >= 10)
            {
                no_of_objcodes -= 10;
                return 10;
            }
            else
            {
                int t = no_of_objcodes;
                no_of_objcodes = 0;
                return t;
            }
        }
    }
    return -1;
}

void main()
{
    inter = fopen("intermediate.txt", "r");
    length = fopen("length.txt", "r");
    counts = fopen("counts.txt", "r");
    objcode = fopen("objcode.txt", "w");

    // head record
    if (!feof(inter))
    {
        fscanf(inter, "%s %s %s %s\n", address, label, opcode, start_address);
        fscanf(length, "%s", prg_len);
        fprintf(objcode, "H");
        put_a_seperator(objcode);
        write_value_as_n_bits_to_the_file(label, 6, objcode, ' ');
        put_a_seperator(objcode);
        write_value_as_n_bits_to_the_file(start_address, 6, objcode, '0');
        put_a_seperator(objcode);
        write_value_as_n_bits_to_the_file(prg_len, 6, objcode, '0');
        put_a_seperator(objcode);
        put_a_newline(objcode);
    }

    // text records
    fscanf(inter, "%s %s %s %s\n", address, label, opcode, operand);
    fscanf(counts, "%i", &no_of_objcodes);
    while (!feof(inter))
    {
        n = no_of_objcodes_to_be_created();
        if (n == 0)
            continue;
        fprintf(objcode, "T");
        put_a_seperator(objcode);
        write_value_as_n_bits_to_the_file(address, 6, objcode, '0');
        put_a_seperator(objcode);
        write_value_as_n_bits_to_the_file(hex(3 * n), 2, objcode, '0');
        put_a_seperator(objcode);
        while (n > 0)
        {
            get_value_from_optab(opcode);
            get_value_from_symtab(operand);
            if (strcmp(opcode_value, hex(0)) != 0) // found in optab
            {
                generate_object_code();
                write_value_as_n_bits_to_the_file(object_code, 6, objcode, '0');
            }
            else if (strcmp(opcode, "WORD") == 0)
            {
                write_value_as_n_bits_to_the_file(hex(atoi(operand)), 6, objcode, '0');
            }
            else if (strcmp(opcode, "BYTE") == 0)
            {
                if (operand[0] == 'C')
                {
                    for (int i = 2; i < strlen(operand) - 1; i++)
                        fprintf(objcode, "%d", operand[i]);
                }
                else
                {
                    char value[5];
                    for (int i = 2; i < strlen(operand) - 1; i++)
                        value[i - 2] = operand[i];
                    fprintf(objcode, "%s", value);
                }
            }
            n--;

            fscanf(inter, "%s %s %s %s\n", address, label, opcode, operand);
           	put_a_seperator(objcode);
        }

        put_a_newline(objcode);
    }

    // end record
    fprintf(objcode, "E");
    put_a_seperator(objcode);
    write_value_as_n_bits_to_the_file(start_address, 6, objcode, '0');
    put_a_seperator(objcode);
    put_a_newline(objcode);

    fclose(counts);
    fclose(inter);
    fclose(objcode);
    fclose(length);
}
