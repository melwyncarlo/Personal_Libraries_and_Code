#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Copyright 2021 Melwyn Francis Carlo */

#define MAX_NUM 1E6

/* This value may  be changed as required.        */
/* This value must be a multiple of 9 (and >= 9). */
#define MAX_DIGITS 9

/* Divide-By-32: Num = Num / 32, and Return = Num Modulo 32. */
int dividebyN(char num[MAX_DIGITS+1], int divisor)
{
    int temp_val  = 0;
    int carry_val = 0;

    for (int i = 0; i < MAX_DIGITS; i++)
    {
        temp_val  = num[i] - '0';
        num[i]    = ((temp_val + (carry_val * 10)) / divisor) + '0';
        carry_val =  (temp_val + (carry_val * 10)) % divisor;
    }

    return carry_val;
}

/* Increment: Num = Num + Offset */
void increment(char num[MAX_DIGITS+1], int offset)
{
    int temp_val = num[MAX_DIGITS-1] - '0' + offset;

    if (temp_val < 10)
    {
        num[MAX_DIGITS-1] = temp_val + '0';
        return;
    }

    int i         = MAX_DIGITS - 1;
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
void summation(char num1[MAX_DIGITS+1], char num2[MAX_DIGITS+1])
{
    int temp_val  = 0;
    int carry_val = 0;

    for (int i = (MAX_DIGITS - 1); i >= 0; i--)
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
void pad(char num[MAX_DIGITS+1])
{
    if (strlen(num) == MAX_DIGITS) return;

    int str_len = (int)strlen(num);
    for (int i = (str_len - 1); i >= 0; i--)
        num[MAX_DIGITS+i-str_len] = num[i];

    memset(&num[0], '0', MAX_DIGITS - str_len);
}

/* File-Seek-Increment: Increment the file pointer by a given offset. */
void file_seek_increment(FILE *file_pointer, 
                         char file_position_offset[MAX_DIGITS+1], 
                         char file_current_position[MAX_DIGITS+1], 
                         char file_size[MAX_DIGITS+1])
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

    char count[MAX_DIGITS+1] = "0";
    pad(count);

    char pos_i[MAX_DIGITS+1] = { 0 };

    while (strcmp(count, file_position_offset) < 0)
    {
        char pre_count[MAX_DIGITS+1] = { 0 };
        sprintf(&pre_count[0], "%d", POWER_REFS[i]);
        pad(pre_count);

        summation(pre_count, count);

        if (strcmp(pre_count, file_position_offset) > 0)
        {
            if (i > 0) i--;
            continue;
        }

        memcpy(&pos_i[0], &file_current_position[0], sizeof(pos_i));
        summation(pos_i, pre_count);

        if (strcmp(pos_i, file_size) >= 0) break;

        fseek(file_pointer, POWER_REFS[i], SEEK_CUR);
        memcpy(&count[0], &pre_count[0], sizeof(count));
    }
}

int main()
{
    /* ---------------------------------------------------------------------- */
    /* Declare and initialize variables.                                      */

    /* ---------------------------------------------------------------------- */
        /* Set the maximum EVEN (NOT ODD) natural number over here. */
        //char n_natural_numbers_str[MAX_DIGITS+1] = "1000";

        
        long int natural_number = 2E7;
        char n_natural_numbers_str[MAX_DIGITS+1] = { 0 };
        sprintf(&n_natural_numbers_str[0], "%ld", natural_number);
        /**/
    /* ---------------------------------------------------------------------- */

    printf("\n File: Create_Primes_Test_N_Numbers_Binary.C\n\n");
    printf(" Generating aa binary-based boolean primes test for natural numbers\n");
    printf(" between 1 and %s.\n\n", n_natural_numbers_str);
    printf(" Copyright 2021 Melwyn Francis Carlo\n\n");
    printf(" ----------------------------------------");
    printf("-----------------------------------\n\n\n");

    pad(n_natural_numbers_str);

    char n_half_natural_numbers_str[MAX_DIGITS+1] = { 0 };
    memcpy(&n_half_natural_numbers_str[0], &n_natural_numbers_str[0], MAX_DIGITS);
    dividebyN(n_half_natural_numbers_str, 2);

    char          zero[MAX_DIGITS+1] = "0";
    char        byte_i[MAX_DIGITS+1] = { 0 };
    char current_i_str[MAX_DIGITS+1] = { 0 };
    char current_n_str[MAX_DIGITS+1] = "3";
    pad(current_n_str);
    pad(zero);

    int bit_i;
    int moduloN;

    char n_doublewords[MAX_DIGITS+1] = { 0 };
    memcpy(&n_doublewords[0], &n_natural_numbers_str[0], MAX_DIGITS);
    moduloN = dividebyN(n_doublewords, 32);
    if (moduloN) increment(n_doublewords, 1);

    char doubleword_i[MAX_DIGITS+1] = { 0 };
    memset(&doubleword_i[0], '0', MAX_DIGITS);
    doubleword_i[MAX_DIGITS-1] = '1';

    uint32_t n_natural_numbers[MAX_DIGITS/9];

    for (int i = 0; i < (MAX_DIGITS / 9); i++)
    {
        char temp_str[11] = { 0 };
        memcpy(&temp_str[0], &n_natural_numbers_str[i*9], 9);

        n_natural_numbers[i] = (uint32_t)atoi(temp_str);
    }

    const int HEADER_SIZE = ((MAX_DIGITS / 9) * 4) + 4;

    char file_size[MAX_DIGITS+1] = { 0 };
    memcpy(&file_size[0], &n_doublewords[0], MAX_DIGITS);
    summation(file_size, n_doublewords);
    summation(file_size, n_doublewords);
    summation(file_size, n_doublewords);
    increment(file_size, HEADER_SIZE);

    const uint32_t MAX_SUB_HEADER_BYTES_UINT32 = (uint32_t)(MAX_DIGITS / 9);

    /* 1789569706d = 0b01101010101010101010101010101010 */
    const uint32_t FIRST_DOUBLEWORD_DATA = 1789569706;
    /* 2863311530d = 0b10101010101010101010101010101010 */
    const uint32_t OTHER_DOUBLEWORD_DATA = 2863311530;

    FILE *fp;

    /* ---------------------------------------------------------------------- */
    /* Prepare file operation.                                                */

    printf(" Creating the 'Prime_Numbers_Test_of_First_N_Natural_Numbers' binary file ...\n");

    fp = fopen("Prime_Numbers_Test_of_First_N_Natural_Numbers.bin", "w+b");

    printf(" Adding header content to the file ...\n");

    fwrite(&MAX_SUB_HEADER_BYTES_UINT32, sizeof(uint32_t), 1, fp);

    for (int i = 0; i < (MAX_DIGITS / 9); i++)
        fwrite(&n_natural_numbers[i], sizeof(uint32_t), 1, fp);

    fwrite(&FIRST_DOUBLEWORD_DATA, sizeof(uint32_t), 1, fp);

    printf(" Setting even numbers in the file contents to boolean false (0) ...\n\n");

    while (strcmp(&doubleword_i[0], &n_doublewords[0]))
    {
        fwrite(&OTHER_DOUBLEWORD_DATA, sizeof(uint32_t), 1, fp);
        increment(doubleword_i, 1);
    }

    moduloN = dividebyN(n_natural_numbers_str, 32);
    if (moduloN) increment(n_natural_numbers_str, 1);

    /* ---------------------------------------------------------------------- */

    printf(" Setting multiples of odd numbers (but not the number itself)\n");
    printf(" in the file contents to boolean false (0).\n\n");

    printf(" From 3 to (%s - 1) ...\n\n", n_half_natural_numbers_str);

    while (strcmp(current_n_str, n_half_natural_numbers_str) < 0)
    {
        printf(" # %s ...\n", current_n_str);

        memcpy(&current_i_str[0], &current_n_str[0], sizeof(current_i_str));

        while (1)
        {
            summation(current_i_str, current_n_str);

            memcpy(&byte_i[0], &current_i_str[0], sizeof(current_i_str));
            moduloN = dividebyN(byte_i, 32);
            bit_i   = moduloN - 1;

            summation(byte_i, byte_i);
            summation(byte_i, byte_i);
            increment(byte_i, HEADER_SIZE);

            if (strcmp(byte_i, file_size) >= 0) break;

            fseek(fp, 0, SEEK_SET);
            file_seek_increment(fp, byte_i, zero, file_size);

            uint32_t byte_data;
            uint32_t byte_data_modification = 0;

            for (int i = 0; i < 32; i++)
            {
                if (i != bit_i) byte_data_modification  |= 1;
                if (i  <    31) byte_data_modification <<= 1;
            }

            if (fread(&byte_data, sizeof(uint32_t), 1, fp))
                byte_data &= byte_data_modification;

            fseek(fp, -4, SEEK_CUR);

            fwrite(&byte_data, sizeof(uint32_t), 1, fp);
        }

        increment(current_n_str, 2);
    }

    fclose(fp);

    printf("\n Done.\n\n");

    return 0;
}
