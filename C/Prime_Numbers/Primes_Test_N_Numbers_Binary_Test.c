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
    /* File preparation and reading file header content.                      */

    int max_n_n = 1;

    FILE *fp;

    fp = fopen("Prime_Numbers_Test_of_First_N_Natural_Numbers.bin", "rb");

    if (fread(&max_n_n, sizeof(uint32_t), 1, fp)) { /* Do Nothing. */ };

    const int MAX_DIGITS = max_n_n * 9;

    char max_n_str[MAX_DIGITS+1];
    memset(&max_n_str, 0, MAX_DIGITS + 1);

    int index = 0;
    uint32_t max_n_i;
    for (int i = 0; i < max_n_n; i++)
    {
        if (fread(&max_n_i, sizeof(uint32_t), 1, fp))
        {
            char temp_str[11] = { 0 };
            sprintf(&temp_str[0], "%d", max_n_i);
            memcpy(&max_n_str[index], &temp_str[0], strlen(temp_str));
            index += strlen(temp_str);
        }
    }

    /* ---------------------------------------------------------------------- */
    /* User interactive input.                                                */

    printf("\n File: Primes_Test_N_Numbers_Binary_Test.C\n");
    printf(" ----------------------------------------");
    printf("-----------------------------------");
    printf("\n Copyright 2021 Melwyn Francis Carlo.\n\n");

    printf("\n Enter a number between 1 and N to check");
    printf("\n whether or not it is a prime number.\n");
    printf("\n N = ");

    int  count = 1;
    char comma = ',';

    const uint32_t MAX_N_STR_LEN = strlen(max_n_str);

    int first_count = -1;
    if (MAX_N_STR_LEN > 3)
        first_count = (MAX_N_STR_LEN % 3);
    if (first_count == 0) first_count = 3;

    int non_zero_term_found = 0;

    uint32_t i = 0;
    while (i < MAX_N_STR_LEN)
    {
        if (i == (MAX_N_STR_LEN - 1))
            comma = 0;

        if (!non_zero_term_found)
            if (max_n_str[i] != '0')
                non_zero_term_found = 1;

        if (count == first_count)
        {
            if (non_zero_term_found) printf("%c%c", max_n_str[i], comma);
            first_count = -1;
            count = 0;
        }
        else if (count == 3)
        {
            if (non_zero_term_found) printf("%c%c", max_n_str[i], comma);
            count = 0;
        }
        else
        {
            if (non_zero_term_found) printf("%c", max_n_str[i]);
        }

        count++;
        i++;
    }

    char one[MAX_DIGITS+1];
    memset(&one[0], 0, MAX_DIGITS + 1);
    one[0] = '1';
    pad(one, MAX_DIGITS);

    char input_num[MAX_DIGITS+1];
    memset(&input_num[0], 0, MAX_DIGITS + 1);
    printf("\n\n Input number :  ");
    if (scanf("%s", input_num) == 1) /* Do Nothing. */

    pad(input_num, MAX_DIGITS);

    int k = 0;
    int input_num_valid = 1;
    while (k < MAX_DIGITS)
    {
        if (!isdigit(input_num[k]))
        {
            input_num_valid = 0;
            break;
        }

        k++;
    }

    pad(max_n_str, MAX_DIGITS);

    if (!input_num_valid)
    {
        printf("\n\n Error: The input number entered is invalid.\n\n\n");
        exit(1);
    }

    if (strcmp(input_num, one) < 0)
    {
        printf("\n\n Error: The input number entered is less than 1.\n\n\n");
        exit(1);
    }

    if (strcmp(input_num, max_n_str) > 0)
    {
        printf("\n\n Error: The input number entered is greater than N.\n\n\n");
        exit(1);
    }

    /* ---------------------------------------------------------------------- */
    /* Main operation - Parsing file contents.                                */

    int moduloN;
    int bit_i;

    const int HEADER_SIZE = (max_n_n * 4) + 4;

    char zero[MAX_DIGITS+1];
    memset(&zero[0], 0, MAX_DIGITS+1);
    zero[0] = '0';
    pad(zero, MAX_DIGITS);

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
    memcpy(&byte_i[0], &input_num[0], MAX_DIGITS+1);
    moduloN = dividebyN(byte_i, 32, MAX_DIGITS);
    bit_i   = 32 - moduloN;

    summation(byte_i, byte_i, MAX_DIGITS);
    summation(byte_i, byte_i, MAX_DIGITS);
    increment(byte_i, HEADER_SIZE, MAX_DIGITS);

    fseek(fp, 0, SEEK_SET);
    file_seek_increment(fp, byte_i, zero, file_size, MAX_DIGITS);

    uint32_t byte_data;

    if (fread(&byte_data, sizeof(uint32_t), 1, fp)) { /* Do Nothing. */ };

    for (int i = 0; i < 32; i++)
    {
        if (i == bit_i) break;
        byte_data >>= 1;
    }

    byte_data &= 1;

    if (byte_data)
        printf("\n Result:  This number is a prime number.\n\n");
    else
        printf("\n Result:  This number is not a prime number.\n\n");

    fclose(fp);

    return 0;
}
