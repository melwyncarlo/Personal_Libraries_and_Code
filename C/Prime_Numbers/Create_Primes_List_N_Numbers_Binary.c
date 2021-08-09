#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Copyright 2021 Melwyn Francis Carlo */

/* Divide-By-N: Num = Num / N, and Return = Num Modulo N. */
int dividebyN(char *num, int divisor, int max_digits)
{
    int temp_val  = 0;
    int carry_val = 0;

    for (int i = 0; i < max_digits; i++)
    {
        temp_val  = num[i] - '0';
        num[i]    = ((temp_val + (carry_val * 10)) / divisor) + '0';
        carry_val =  (temp_val + (carry_val * 10)) % divisor;
    }

    return carry_val;
}

/* Increment: Num = Num + Offset */
void increment(char *num, int offset, int max_digits)
{
    int temp_val = num[max_digits-1] - '0' + offset;

    if (temp_val < 10)
    {
        num[max_digits-1] = temp_val + '0';
        return;
    }

    int i         = max_digits - 1;
    int carry_val = offset;

    while (carry_val)
    {
        temp_val  =  num[i]   - '0' + carry_val;
        carry_val =  temp_val / 10;
        num[i]    = (temp_val % 10) + '0';

        if (i-- == 0)
        {
            if (carry_val)
            {
                printf("\n\n Error: The global constant parameter 'MAX_DIGITS' ");
                printf("limit\n        has been exceeded.\n\n");
                exit(136);
            }
        }
    }
}

/* Summation: Num-1 =  Num-1 + Num-2 */
void summation(char *num1, char *num2, int max_digits)
{
    int temp_val  = 0;
    int carry_val = 0;

    for (int i = (max_digits - 1); i >= 0; i--)
    {
        temp_val  =  num1[i] - '0' + num2[i] - '0' + carry_val;
        carry_val =  temp_val / 10;
        num1[i]   = (temp_val % 10) + '0';

        if (i == 0)
        {
            if (carry_val)
            {
                printf("\n\n Error: The global constant parameter 'MAX_DIGITS' ");
                printf("limit\n        has been exceeded.\n\n");
                exit(136);
            }
        }
    }
}

/* Pad a number with leading zeroes. */
void pad(char *num, int max_digits)
{
    if ((int)strlen(num) == max_digits) return;

    int str_len = (int)strlen(num);
    for (int i = (str_len - 1); i >= 0; i--)
        num[max_digits+i-str_len] = num[i];

    memset(&num[0], '0', max_digits - str_len);
}

/* File-Seek-Increment: Increment the file pointer by a given offset. */
void file_seek_increment(FILE *file_pointer, 
                         char *file_position_offset, 
                         char *file_current_position, 
                         char *file_size, 
                         int   max_digits)
{
    const int POWER_REFS[9] = { 1, 
                                10, 
                                100, 
                                1000, 
                                10000, 
                                100000, 
                                1000000, 
                                10000000, 
                                100000000 };
    int i = 8;

    char count[max_digits+1];
    memset(&count[0], 0, max_digits+1);
    count[0] = '0';
    pad(count, max_digits);

    char pos_i[max_digits+1];
    pos_i[max_digits] = 0;

    while (strcmp(count, file_position_offset) < 0)
    {
        char pre_count[max_digits+1];
        memset(&pre_count[0], 0, max_digits+1);
        sprintf(&pre_count[0], "%d", POWER_REFS[i]);
        pad(pre_count, max_digits);

        summation(pre_count, count, max_digits);

        if (strcmp(pre_count, file_position_offset) > 0)
        {
            if (i > 0) i--;
            continue;
        }

        memcpy(&pos_i[0], &file_current_position[0], sizeof(pos_i));
        summation(pos_i, pre_count, max_digits);

        if (strcmp(pos_i, file_size) >= 0) break;

        fseek(file_pointer, POWER_REFS[i], SEEK_CUR);
        memcpy(&count[0], &pre_count[0], sizeof(count));
    }
}

int main()
{
    /* ---------------------------------------------------------------------- */
    /* File preparation and reading and writing file header content.          */

    uint32_t max_n_n_uint32;

    int max_n_n = 1;

    FILE *fpi;
    FILE *fpo;

    fpi = fopen("Prime_Numbers_Test_of_First_N_Natural_Numbers.bin", "rb");
    fpo = fopen("Prime_Numbers_List_of_First_N_Natural_Numbers.bin", "wb");

    if (fread(&max_n_n_uint32, sizeof(uint32_t), 1, fpi))
    {
        max_n_n = max_n_n_uint32;
        fwrite(&max_n_n_uint32, sizeof(uint32_t), 1, fpo);
    }

    const int MAX_DIGITS = max_n_n * 9;

    char max_n_str[MAX_DIGITS+1];
    memset(&max_n_str, 0, MAX_DIGITS + 1);

    int index = 0;
    uint32_t max_n_i;
    for (int i = 0; i < max_n_n; i++)
    {
        if (fread(&max_n_i, sizeof(uint32_t), 1, fpi))
        {
            char temp_str[11] = { 0 };
            sprintf(&temp_str[0], "%d", max_n_i);
            memcpy(&max_n_str[index], &temp_str[0], strlen(temp_str));
            index += strlen(temp_str);

            fwrite(&max_n_i, sizeof(uint32_t), 1, fpo);
        }
    }

    for (int i = 0; i < max_n_n; i++)
    {
        uint32_t dummy_number = 0;
        fwrite(&dummy_number, sizeof(uint32_t), 1, fpo);
    }

    printf("\n File: Create_Primes_List_N_Numbers_Binary.C\n\n");
    printf(" Generating a binary-based prime numbers list for natural numbers\n");
    printf(" between 1 and %s.\n\n", max_n_str);
    printf(" Copyright 2021 Melwyn Francis Carlo\n\n");
    printf(" ----------------------------------------");
    printf("-----------------------------------\n\n\n");

    /* ---------------------------------------------------------------------- */
    /* Main operation - Parsing file contents.                                */

    printf(" Reading  the 'Prime_Numbers_Test_of_First_N_Natural_Numbers' binary file ...\n");
    printf(" Creating the 'Prime_Numbers_List_of_First_N_Natural_Numbers' binary file ...\n");
    printf(" Adding header content to the output file ...\n\n");

    printf(" Testing the input file contents for prime numbers\n");
    printf(" and appending the numbers to the output file ...\n\n");

    printf(" The prime numbers are:\n\n");

    pad(max_n_str, MAX_DIGITS);

    int moduloN;

    const int HEADER_SIZE = (max_n_n * 4) + 4;

    char n_doublewords[MAX_DIGITS+1];
    memset(&n_doublewords[0], 0, MAX_DIGITS+1);
    memcpy(&n_doublewords[0], &max_n_str[0], strlen(max_n_str));
    pad(n_doublewords, MAX_DIGITS);
    moduloN = dividebyN(n_doublewords, 32, MAX_DIGITS);
    if (moduloN) increment(n_doublewords, 1, MAX_DIGITS);

    char file_size[MAX_DIGITS+1];
    memcpy(&file_size[0], &n_doublewords[0], MAX_DIGITS+1);
    summation(file_size, n_doublewords, MAX_DIGITS);
    summation(file_size, n_doublewords, MAX_DIGITS);
    summation(file_size, n_doublewords, MAX_DIGITS);
    increment(file_size, HEADER_SIZE, MAX_DIGITS);

    char byte_i[MAX_DIGITS+1];
    memset(&byte_i[0], 0, MAX_DIGITS+1);
    sprintf(&byte_i[0], "%d", HEADER_SIZE);
    pad(byte_i, MAX_DIGITS);

    fseek(fpi, HEADER_SIZE, SEEK_SET);

    char n_count[MAX_DIGITS+1];
    memset(&n_count[0], 0, MAX_DIGITS + 1);
    n_count[0] = '1';
    pad(n_count, MAX_DIGITS);

    char i_count[MAX_DIGITS+1];
    memset(&i_count[0], 0, MAX_DIGITS + 1);
    i_count[0] = '0';
    pad(i_count, MAX_DIGITS);

    int may_exit_loop = 0;

    while (strcmp(byte_i, file_size) < 0)
    {
        uint32_t byte_data_ref;

        if (fread(&byte_data_ref, sizeof(uint32_t), 1, fpi)) { /* Do Nothing. */ };

        for (int bit_i = 0; bit_i < 32; bit_i++)
        {
            uint32_t byte_data_temp = byte_data_ref;

            for (int i = 31; i >= 0; i--)
            {
                if (i == bit_i) break;
                byte_data_temp >>= 1;
            }

            byte_data_temp &= 1;

            if (byte_data_temp)
            {
                if (strcmp(n_count, max_n_str) > 0)
                {
                    may_exit_loop = 1;
                    break;
                }

                printf(" %s\n", n_count);

                increment(i_count, 1, MAX_DIGITS);

                for (int i = 0; i < max_n_n; i++)
                {
                    char temp_str[10] = { 0 };
                    memcpy(&temp_str[0], &n_count[i*9], 9);

                    uint32_t byte_data = atoi(temp_str);

                    fwrite(&byte_data, sizeof(uint32_t), 1, fpo);
                }
            }

            increment(n_count, 1, MAX_DIGITS);
        }

        if (may_exit_loop) break;

        increment(byte_i, 4, MAX_DIGITS);
    }

    fseek(fpo, HEADER_SIZE, SEEK_SET);

    for (int i = 0; i < max_n_n; i++)
    {
        char temp_str[11] = { 0 };
        memcpy(&temp_str[0], &i_count[i*9], 9);

        uint32_t number_of_primes_data = (uint32_t)atoi(temp_str);
        fwrite(&number_of_primes_data, sizeof(uint32_t), 1, fpo);
    }

    fclose(fpi);
    fclose(fpo);

    printf("\n Done.\n\n");

    return 0;
}
