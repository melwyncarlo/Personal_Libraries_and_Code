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

/* Increment: Num = Num + Offset */
void decrement(char *num, int max_digits)
{
    num[max_digits-1] = num[max_digits-1] - 1;

    if (num[max_digits-1] >= '0') return;

    int i = max_digits;

    while (num[--i] < '0')
    {
        if (i == 0)
        {
            printf("\n\n Error: The global constant parameter 'MAX_DIGITS' ");
            printf("limit\n        has been exceeded.\n\n");
            exit(136);
        }

        num[i] += 10;
        num[i-1]--;
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

    uint32_t max_n_n_uint32;

    int max_n_n = 1;

    FILE *fp;

    fp = fopen("Prime_Numbers_List_of_First_N_Natural_Numbers.bin", "rb");

    if (fread(&max_n_n_uint32, sizeof(uint32_t), 1, fp)) { /* Do Nothing. */ };

    max_n_n = (int)max_n_n_uint32;

    const int MAX_DIGITS = max_n_n * 9;

    char max_n_str[MAX_DIGITS+1];
    memset(&max_n_str, 0, MAX_DIGITS + 1);

    int index = 0;
    uint32_t temp_val_uint32;
    for (int i = 0; i < max_n_n; i++)
    {
        if (fread(&temp_val_uint32, sizeof(uint32_t), 1, fp))
        {
            if ((i == 0) && (temp_val_uint32 == 0)) continue;

            char temp_str[11] = { 0 };
            sprintf(&temp_str[0], "%d", temp_val_uint32);
            memcpy(&max_n_str[index], &temp_str[0], strlen(temp_str));
            index += strlen(temp_str);
        }
    }

    const uint32_t MAX_N_STR_LEN = strlen(max_n_str);

    char primes_n_str[MAX_DIGITS+1];
    memset(&primes_n_str, 0, MAX_DIGITS + 1);

    index = 0;
    for (int i = 0; i < max_n_n; i++)
    {
        if (fread(&temp_val_uint32, sizeof(uint32_t), 1, fp))
        {
            char temp_str[11] = { 0 };
            sprintf(&temp_str[0], "%d", temp_val_uint32);
            memcpy(&primes_n_str[index], &temp_str[0], strlen(temp_str));
            index += strlen(temp_str);
        }
    }

    /* ---------------------------------------------------------------------- */
    /* User interactive input.                                                */

    printf("\n File: Primes_List_N_Numbers_Binary_Test.C\n");
    printf(" ----------------------------------------");
    printf("-----------------------------------");
    printf("\n Copyright 2021 Melwyn Francis Carlo.\n\n");

    printf("\n Between the natural numbers 1 and N, there exists M prime numbers.\n");

    printf("\n M = ");

    int  count = 1;
    char comma = ',';

    int first_count = -1;
    if (strlen(primes_n_str) > 3)
        first_count = (strlen(primes_n_str) % 3);
    if (first_count == 0) first_count = 3;

    uint32_t i = 0;
    while (i < strlen(primes_n_str))
    {
        if (i == (strlen(primes_n_str) - 1))
            comma = 0;

        if (count == first_count)
        {
            printf("%c%c", primes_n_str[i], comma);
            first_count = -1;
            count = 0;
        }
        else if (count == 3)
        {
            printf("%c%c", primes_n_str[i], comma);
            count = 0;
        }
        else
        {
            printf("%c", primes_n_str[i]);
        }

        count++;
        i++;
    }

    printf("\n N = ");

    count = 1;
    comma = ',';

    first_count = -1;
    if (MAX_N_STR_LEN > 3)
        first_count = (MAX_N_STR_LEN % 3);
    if (first_count == 0) first_count = 3;

    i = 0;
    while (i < MAX_N_STR_LEN)
    {
        if (i == (MAX_N_STR_LEN - 1))
            comma = 0;

        if (count == first_count)
        {
            printf("%c%c", max_n_str[i], comma);
            first_count = -1;
            count = 0;
        }
        else if (count == 3)
        {
            printf("%c%c", max_n_str[i], comma);
            count = 0;
        }
        else
        {
            printf("%c", max_n_str[i]);
        }

        count++;
        i++;
    }

    printf("\n\n Do you wish to:");
    printf("\n 1. Obtain the Ith prime number.");
    printf("\n 2. Obtain a list of prime numbers between I1 and I2.\n");

    pad(max_n_str,    MAX_DIGITS);
    pad(primes_n_str, MAX_DIGITS);

    char one[MAX_DIGITS+1];
    memset(&one[0], 0, MAX_DIGITS+1);
    one[0] = '1';
    pad(one, MAX_DIGITS);

    char input_choice;
    char input_i1[MAX_DIGITS+1];
    char input_i2[MAX_DIGITS+1];

    memset(&input_i1[0], 0, MAX_DIGITS+1);
    memset(&input_i2[0], 0, MAX_DIGITS+1);

    printf("\n Enter 1 or 2 :  ");
    if (scanf("%s", &input_choice) == 1) { /* Do Nothing. */ }

    if (input_choice == '1')
    {
        printf("\n Enter the Ith value :  ");
        if (scanf("%s", input_i1) == 1) { /* Do Nothing. */ }
    }
    else if (input_choice == '2')
    {
        printf("\n Enter the I1 value :  ");
        if (scanf("%s", input_i1) == 1) { /* Do Nothing. */ }

        printf("\n Enter the I2 value :  ");
        if (scanf("%s", input_i2) == 1) { /* Do Nothing. */ }
    }
    else
    {
        printf("\n\n Error: The input choice entered is invalid.\n\n\n");
        exit(1);
    }

    pad(input_i1, MAX_DIGITS);

    int k = 0;
    int input_i1_valid = 1;
    while (k < MAX_DIGITS)
    {
        if (!isdigit(input_i1[k]))
        {
            input_i1_valid = 0;
            break;
        }

        k++;
    }

    char ith_or_i1[3][4] = { "", "Ith", "I1" };

    if (!input_i1_valid)
    {
        printf("\n\n Error: The %s value entered is invalid.\n\n\n", 
                ith_or_i1[input_choice - '0']);
        exit(1);
    }

    if (strcmp(input_i1, one) < 0)
    {
        printf("\n\n Error: The %s value entered is less than 1.\n\n\n", 
                ith_or_i1[input_choice - '0']);
        exit(1);
    }

    if (input_choice == '1')
    {
        if (strcmp(input_i1, primes_n_str) > 0)
        {
            printf("\n\n Error: The %s value entered is greater than M.\n\n\n", 
                    ith_or_i1[input_choice - '0']);
            exit(1);
        }
    }
    else /* if (input_choice == '2') */
    {
        if (strcmp(input_i1, max_n_str) > 0)
        {
            printf("\n\n Error: The %s value entered is greater than N.\n\n\n", 
                    ith_or_i1[input_choice - '0']);
            exit(1);
        }
    }

    if (input_choice == '2')
    {
        pad(input_i2, MAX_DIGITS);

        k = 0;
        int input_i2_valid = 1;
        while (k < MAX_DIGITS)
        {
            if (!isdigit(input_i2[k]))
            {
                input_i2_valid = 0;
                break;
            }

            k++;
        }

        if (!input_i2_valid)
        {
            printf("\n\n Error: The I2 value entered is invalid.\n\n\n");
            exit(1);
        }

        if (strcmp(input_i2, one) < 0)
        {
            printf("\n\n Error: The I2 value entered is less than 1.\n\n\n");
            exit(1);
        }

        if (strcmp(input_i2, max_n_str) > 0)
        {
            printf("\n\n Error: The I2 value entered is greater than N.\n\n\n");
            exit(1);
        }

        if (strcmp(input_i1, input_i2) > 0)
        {
            printf("\n\n Error: The I1 value entered is greater than the I2 value.\n\n\n");
            exit(1);
        }
    }

    /* ---------------------------------------------------------------------- */
    /* Main operation - Parsing file contents.                                */

    const int HEADER_SIZE = (max_n_n * 4) + (max_n_n * 4) + 4;

    char zero[MAX_DIGITS+1];
    memset(&zero[0], 0, MAX_DIGITS+1);
    zero[0] = '0';
    pad(zero, MAX_DIGITS);

    char file_size[MAX_DIGITS+1];
    memcpy(&file_size[0], &primes_n_str[0], MAX_DIGITS+1);
    summation(file_size, file_size, MAX_DIGITS);
    summation(file_size, file_size, MAX_DIGITS);

    char initial_file_size[MAX_DIGITS+1];
    memcpy(&initial_file_size[0], &file_size[0], MAX_DIGITS);

    for (k = 1; k < max_n_n; k++)
        summation(file_size, initial_file_size, MAX_DIGITS);

    increment(file_size, HEADER_SIZE, MAX_DIGITS);

    char byte_i[MAX_DIGITS+1];
    char initial_byte_i[MAX_DIGITS+1];

    if (input_choice == '1')
    {
        memcpy(&byte_i[0], &input_i1[0], MAX_DIGITS+1);

        decrement(byte_i,         MAX_DIGITS);
        summation(byte_i, byte_i, MAX_DIGITS);
        summation(byte_i, byte_i, MAX_DIGITS);

        memcpy(&initial_byte_i[0], &byte_i[0], MAX_DIGITS);

        for (k = 1; k < max_n_n; k++)
            summation(byte_i, initial_byte_i, MAX_DIGITS);

        increment(byte_i, HEADER_SIZE, MAX_DIGITS);

        fseek(fp, 0, SEEK_SET);
        file_seek_increment(fp, byte_i, zero, file_size, MAX_DIGITS);

        printf("\n Result:  ");

        int index = 0;
        uint32_t byte_data;
        for (int i = 0; i < max_n_n; i++)
        {
            if (fread(&byte_data, sizeof(uint32_t), 1, fp))
            {
                char byte_data_str[10] = { 0 };
                sprintf(&byte_data_str[0], "%d", byte_data);
                printf("%s", byte_data_str);
                index += strlen(byte_data_str);
            }
        }

        printf("\n\n");
    }
    else /* if (input_choice == '2') */
    {
        fseek(fp, HEADER_SIZE, SEEK_SET);

        char prime_num_str[MAX_DIGITS+1];
        memset(&prime_num_str[0], 0, MAX_DIGITS + 1);
        char prime_num_str_padded[MAX_DIGITS+1];
        memset(&prime_num_str_padded[0], 0, MAX_DIGITS + 1);

        char previous_prime_num_str[MAX_DIGITS+1];
        previous_prime_num_str[MAX_DIGITS] = 0;
        previous_prime_num_str[0] = '0';
        previous_prime_num_str[1] =  0;
        pad(previous_prime_num_str, MAX_DIGITS);

        char i_count[MAX_DIGITS+1];
        i_count[MAX_DIGITS] = 0;
        i_count[0] = '1';
        i_count[1] =  0;
        pad(i_count, MAX_DIGITS);

        while (1)
        {
            int index = 0;
            uint32_t byte_data;
            for (int i = 0; i < max_n_n; i++)
            {
                if (fread(&byte_data, sizeof(uint32_t), 1, fp))
                {
                    char temp_str[11] = { 0 };
                    sprintf(&temp_str[0], "%d", byte_data);
                    memcpy(&prime_num_str[index], &temp_str[0], strlen(temp_str));
                    index += strlen(temp_str);
                }
            }

            memcpy(&prime_num_str_padded[0], &prime_num_str[0], MAX_DIGITS);
            pad(prime_num_str_padded, MAX_DIGITS);

            if (!strcmp(prime_num_str_padded, previous_prime_num_str)) break;

            memcpy(&previous_prime_num_str[0], &prime_num_str_padded[0], MAX_DIGITS);

            if (strcmp(prime_num_str_padded, input_i1) >= 0) break;
        }

        const char MULTIPLE_SPACES[11] = "          ";

        if (strcmp(prime_num_str_padded, input_i1) < 0)
        {
            printf("\n Result:  No prime numbers exist within this range.\n\n");
            fclose(fp);
            return 0;
        }

        if (strcmp(prime_num_str_padded, input_i2) > 0)
        {
            printf("\n Result:  No prime numbers exist within this range.\n\n");
            fclose(fp);
            return 0;
        }

        printf("\n Result:  The prime numbers are:\n\n");

        printf(" %s%s\n", MULTIPLE_SPACES, prime_num_str);

        while (1)
        {
            int index = 0;
            uint32_t byte_data;
            for (int i = 0; i < max_n_n; i++)
            {
                if (fread(&byte_data, sizeof(uint32_t), 1, fp))
                {
                    char temp_str[11] = { 0 };
                    sprintf(&temp_str[0], "%d", byte_data);
                    memcpy(&prime_num_str[index], &temp_str[0], strlen(temp_str));
                    index += strlen(temp_str);
                }
            }

            memcpy(&prime_num_str_padded[0], &prime_num_str[0], MAX_DIGITS);
            pad(prime_num_str_padded, MAX_DIGITS);

            if (!strcmp(prime_num_str_padded, previous_prime_num_str)) break;

            memcpy(&previous_prime_num_str[0], &prime_num_str_padded[0], MAX_DIGITS);

            if (strcmp(prime_num_str_padded, input_i2) > 0) break;

            increment(i_count, 1, MAX_DIGITS);

            printf(" %s%s\n", MULTIPLE_SPACES, prime_num_str);
        }

        int str_index = 0;
        while (i_count[str_index] == '0') str_index++;

        char i_count_modified[MAX_DIGITS+1];
        i_count_modified[MAX_DIGITS-str_index] = 0;
        memcpy(&i_count_modified, &i_count[str_index], MAX_DIGITS - str_index);

        printf("\n%s%s prime numbers have been obtained.\n\n", 
                MULTIPLE_SPACES, i_count_modified);
    }

    fclose(fp);

    return 0;
}
