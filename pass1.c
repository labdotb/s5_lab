#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

char hexadecimalnum[5];
char *hex(int decimalnum)
{
    long quotient, remainder;
    int j = 0;
    char temp;
    quotient = decimalnum;
    for (int i = 0; i < 5; i++)
        hexadecimalnum[i] = '\000';
    while (quotient > 0)
    {
        remainder = quotient % 16;
        if (remainder < 10)
            hexadecimalnum[j++] = 48 + remainder;
        else
            hexadecimalnum[j++] = 55 + remainder;
        quotient = quotient / 16;
    }
    for (int i = 0; i < j / 2; i++)
    {
        temp = hexadecimalnum[i];
        hexadecimalnum[i] = hexadecimalnum[j - i - 1];
        hexadecimalnum[j - i - 1] = temp;
    }
    return hexadecimalnum;
}

int dec(char *hex)
{
    int dec = 0, len = strlen(hex) - 1, val, power = 0;
    while (len >= 0)
    {
        if (hex[len] >= '0' && hex[len] <= '9')
            val = hex[len] - 48;
        else
            val = hex[len] - 65 + 10;
        if (power == 0)
            power = 1;
        else
            power *= 16;
        dec += val * power;
        len--;
    }
    return dec;
}

int found_in_optab(char *x)
{
    FILE *optab = fopen("optab.txt", "r");
    char opcode[10], value[5];
    while (!feof(optab))
    {
        fscanf(optab, "%s %s\n", opcode, value);
        if (strcmp(x, opcode) == 0)
        {
            fclose(optab);
            return 1;
        }
    }
    return 0;
}

void main()
{
    char opcode[10], label[10], operand[10];
    int locctr, start, len = 0, count = 0;
    FILE *input, *symtab, *inter, *length, *counts;
    input = fopen("input.txt", "r");
    symtab = fopen("symtab.txt", "w");
    inter = fopen("intermediate.txt", "w");
    length = fopen("length.txt", "w");
    counts = fopen("counts.txt", "w");

    while (!feof(input))
    {
        fscanf(input, "%s %s %s\n", label, opcode, operand);
        if (strcmp(opcode, "START") == 0)
        {
            start = dec(operand);
            locctr = start;
            fprintf(inter, "* %s %s %s\n", label, opcode, hex(locctr));
            if (strcmp(label, "*") != 0)
            {
                fprintf(symtab, "%s %s\n", label, hex(locctr));
            }
        }
        else
        {
            count++;
            if (strcmp(label, "*") != 0)
            {
                fprintf(symtab, "%s %s\n", label, hex(locctr));
            }
            if (found_in_optab(opcode))
            {
                fprintf(inter, "%s %s %s %s\n", hex(locctr), label, opcode, operand);
                locctr += 3;
            }
            else
            {
                if (strcmp(opcode, "RESW") == 0)
                {
                    fprintf(inter, "%s %s %s %s\n", hex(locctr), label, opcode, operand);
                    locctr += (3 * atoi(operand));
                    fprintf(counts, "%d ", --count);
                    count = 0;
                }
                else if (strcmp(opcode, "RESB") == 0)
                {
                    fprintf(inter, "%s %s %s %s\n", hex(locctr), label, opcode, operand);
                    locctr += atoi(operand);
                    fprintf(counts, "%d ", --count);
                    count = 0;
                }
                else if (strcmp(opcode, "BYTE") == 0)
                {
                    fprintf(inter, "%s %s %s %s\n", hex(locctr), label, opcode, operand);
                    if (operand[0] == 'X')
                        locctr += (strlen(operand) - 3) / 2;
                    else
                        locctr += strlen(operand) - 3;
                }
                else if (strcmp(opcode, "WORD") == 0)
                {
                    fprintf(inter, "%s %s %s %s\n", hex(locctr), label, opcode, operand);
                    locctr += 3;
                }
                else
                {
                    printf("\n Error\n");
                    exit(0);
                }
            }
        }
    }
    if (count != 0)
        fprintf(counts, "%d ", count);
    len = locctr - start;
    fprintf(length, "%s", hex(len));
    
    fclose(counts);
    fclose(input);
    fclose(symtab);
    fclose(inter);
    fclose(length);
}
