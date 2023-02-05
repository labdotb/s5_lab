#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    char name[30];
    int address;
    int update_add[50];
    int count;
} symboltable_t;

typedef struct
{
    unsigned char record[300];
    int objcount;
    int len;
    int address;
} textrecord_t;

textrecord_t textrecords[1000];
symboltable_t symbol[20];

int cc = 0;
int ss = 0;

char *pad(char *s1)
{
    char str[30] = "";
    char pad[] = " ";
    if (strlen(s1) >= 6)
        return s1;
    strcat(str, s1);
    for (int i = 0; i < (6 - strlen(s1)); i++)
    {
        strcat(str, pad);
    }
    s1 = str;
    return s1;
}

void reset()
{
    for (int i = 0; i < 1000; i++)
    {
        textrecords[i].record[0] = '\000';
        textrecords[i].objcount = 0;
        textrecords[i].len = 0;
        textrecords[i].address = 0;
    }
    for (int i = 0; i < 20; i++)
    {
        symbol[i].name[0] = '\0';
        symbol[i].address = -1;
        symbol[i].count = 0;
    }
    ss = 0;
    cc = 0;
}

void write_end()
{
    textrecords[cc].record[textrecords[cc].objcount] = '^';
    textrecords[cc].objcount++;
}

void create_new_record_add(int add)
{
    cc++;
    textrecords[cc].address = add;
}

void write_byte(int byte)
{
    if (textrecords[cc].len > 29)
    {
        write_end();
        create_new_record_add(textrecords[cc].address + textrecords[cc].len);
    }
    textrecords[cc].record[textrecords[cc].objcount] = (unsigned char)byte;
    textrecords[cc].len++;
    textrecords[cc].objcount++;
}

void write_instruction(int op, int add)
{
    write_byte(op);
    write_byte(add >> 8);
    write_byte((add << 24) >> 24);
}

int symbol_exist(char name[30])
{
    for (int i = 0; i < ss; i++)
    {
        if (!strcmp(symbol[i].name, name))
            return i;
    }
    return -1;
}

void insert_modification(int address, int data)
{
    int new = textrecords[cc].address + textrecords[cc].len;
    create_new_record_add(address);
    write_byte(data >> 8);
    write_byte((data << 24) >> 24);
    write_end();
    create_new_record_add(new);
}

void insert_symbol(char name[30], int address, int place)
{
    int index = symbol_exist(name);
    if (index > 0)
    {
        if (address < 0 && symbol[index].address < 0)
        {
            symbol[index].update_add[symbol[index].count++] = place;
        }
        else if (address > 0 && symbol[index].address < 0)
        {
            symbol[index].address = address;
            for (int i = 0; i < symbol[index].count; i++)
            {
                insert_modification(symbol[index].update_add[i], symbol[index].address);
            }
        }
    }
    else
    {
        strcpy(symbol[ss].name, name);
        if (address < 0)
        {
            symbol[ss].update_add[symbol[ss].count++] = place;
        }
        else
        {
            symbol[ss].address = address;
        }
        ss++;
    }
}

void write_text(FILE *file)
{
    for (int i = 0; i < 1000; i++)
    {
        if (textrecords[i].len > 0)
        {
            fprintf(file, "T^%.06x^%.02x^", textrecords[i].address, textrecords[i].len);
            for (int j = 0; j < textrecords[i].objcount; j++)
            {
                if (textrecords[i].record[j] == '^')
                {
                    fprintf(file, "^");
                }
                else
                {
                    fprintf(file, "%.02x", textrecords[i].record[j]);
                }
            }
            fprintf(file, "\n");
        }
    }
}

void main()
{
    reset();
    FILE *input, *optab, *output;
    int locctr, startadd, op1, o, len, opobj = 0;
    char opcode[20], label[20], operand[20], otp[20], pair[3], progname[30];
    input = fopen("input.txt", "r");
    output = fopen("record.txt", "w");
    optab = fopen("optab.txt", "r");
    fscanf(input, "%s%s%x", progname, opcode, &op1);
    if (strcmp(opcode, "START") == 0)
    {
        startadd = op1;
        locctr = startadd;
    }
    else
    {
        locctr = 0;
    }
    fscanf(input, "%s%s%s", label, opcode, operand);
    textrecords[0].address = startadd;
    while (!feof(input))
    {
        if (strcmp(label, "**") != 0)
        {
            insert_symbol(label, locctr, -1);
        }
        if (strcmp(opcode, "END") == 0)
        {
            printf("program length=%x\n", (locctr - startadd));
            break;
        }
        fscanf(optab, "%s%x", otp, &o);
        while (!feof(optab))
        {
            if (strcmp(opcode, otp) == 0)
            {
                opobj = o;
                int index = symbol_exist(operand);
                if (index > -1)
                {
                    write_instruction(opobj, symbol[index].address);
                    write_end();
                }
                else
                {
                    insert_symbol(operand, -1, locctr + 1);
                    write_instruction(opobj, 0);
                    write_end();
                }
                locctr += 3;
                break;
            }
            fscanf(optab, "%s%x", otp, &o);
        }
        fseek(optab, 0L, SEEK_SET);
        if (!strcmp(opcode, "WORD"))
        {
            int value = atoi(operand);
            write_byte(value >> 16);
            write_byte((value << 8) >> 24);
            write_byte((value << 24) >> 24);
            write_end();
            locctr += 3;
        }
        else if (!strcmp(opcode, "RESW"))
        {
            int op1 = atoi(operand);
            create_new_record_add(textrecords[cc].address + textrecords[cc].len + 3 * op1);
            locctr += 3 * op1;
        }
        else if (!strcmp(opcode, "BYTE"))
        {
            if (operand[0] == 'X')
            {
                pair[2] = '\0';
                for (int i = 2; i < (strlen(operand) - 2); i += 2)
                {
                    strncpy(pair, operand + i, 2);
                    write_byte((int)strtol(pair, NULL, 16));
                }
                write_end();
                locctr = locctr + len / 2;
            }
            else
            {
                for (int i = 2; i < (strlen(operand) - 1); i++)
                {
                    write_byte(operand[i]);
                }
                write_end();
                locctr = locctr + strlen(operand) - 3;
            }
        }
        else if (!strcmp(opcode, "RESB"))
        {
            int op1 = atoi(operand);
            locctr = locctr + op1;
            create_new_record_add(textrecords[cc].address + textrecords[cc].len + op1);
        }
        fscanf(input, "%s%s%s", label, opcode, operand);
    }
    fprintf(output, "H^%.6s^%06x^%06x^\n", pad(progname), startadd, (locctr - startadd));
    write_text(output);
    fprintf(output, "E^%.06x^\n", textrecords[0].address);
}
