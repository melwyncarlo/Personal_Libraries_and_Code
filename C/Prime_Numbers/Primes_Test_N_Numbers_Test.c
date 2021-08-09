#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Copyright 2021 Melwyn Francis Carlo */

/* Increment: Num = Num + Offset */
void increment(char *num, int max_digits, int offset)
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

/* Difference: Num-1 =  Num-1 - Num-2 */
void difference(char *num1, char *num2, int max_digits)
{
    for (int i = (max_digits - 1); i >= 0; i--)
    {
        num1[i] -= num2[i] - '0';
        if (num1[i] < '0')
        {
            if (i == 0)
            {
                printf("\n\n Error: The global constant parameter 'MAX_DIGITS' ");
                printf("limit\n        has been exceeded.\n\n");
                exit(136);
            }
            num1[i]   += 10;
            num1[i-1] -=  1;
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

int main()
{
    /* ---------------------------------------------------------------------- */
    /* File preparation and reading file header content.                      */

    char max_digits_str[11] = { 0 };
    uint32_t max_digits     =  10;

    FILE *fp;

    fp = fopen("Prime_Numbers_Test_of_First_N_Natural_Numbers", "r");

    if (fgets(max_digits_str, 11, fp) != NULL)
        max_digits = atoi(max_digits_str);
    
    char max_n_str[max_digits+1];
    max_n_str[max_digits] = 0;

    if (fgets(max_n_str, max_digits + 1, fp) != NULL) /* Do Nothing. */

    fseek(fp, 10 + max_digits, SEEK_SET);

    /* ---------------------------------------------------------------------- */
    /* User interactive input.                                                */

    printf("\n File: Primes_Test_N_Numbers_Test.C\n");
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

    char one[max_digits+1];
    memset(&one[0], 0, max_digits+1);
    one[0] = '1';
    pad(one, max_digits);

    char input_num[max_digits+1];
    memset(&input_num[0], 0, max_digits+1);
    printf("\n\n Input number :  ");
    if (scanf("%s", input_num) == 1) /* Do Nothing. */

    pad(input_num, max_digits);

    i = 0;
    int input_num_valid = 1;
    while (i < max_digits)
    {
        if (!isdigit(input_num[i]))
        {
            input_num_valid = 0;
            break;
        }

        i++;
    }

    pad(max_n_str, max_digits);

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

    char current_n[max_digits+1];
    memset(&current_n[0], '0', max_digits);
    current_n[max_digits]   =  0;
    current_n[max_digits-1] = '1';

    char difference_n[max_digits+1];
    memset(&difference_n[0], '0', max_digits);
    difference_n[max_digits] = 0;

    const int power_refs[10] = { 1, 
                                 10, 
                                 100, 
                                 1000, 
                                 10000, 
                                 100000, 
                                 1000000, 
                                 10000000, 
                                 100000000, 
                                 1000000000 };

    char power_refs_str[10][max_digits+1];
    for (i = 0; i < 10; i++)
    {
        int power_val = 1;
        if (i > 0)
            for (uint32_t j = 0; j < i; j++)
                power_val *= 10;

        power_refs_str[i][max_digits] = 0;
        sprintf(&power_refs_str[i][0], "%d", power_val);
        pad(power_refs_str[i], max_digits);
    }

    i = 9;
    while (strcmp(current_n, input_num))
    {
        memcpy(&difference_n[0], &input_num[0], max_digits);
        difference(difference_n, current_n, max_digits);
        if (strcmp(power_refs_str[i], difference_n) > 0)
        {
            if (i > 0) i--;
            continue;
        }

        fseek(fp, power_refs[i], SEEK_CUR);
        increment(current_n, max_digits, power_refs[i]);
    }

    char is_prime = fgetc(fp);

    if (is_prime == '1')
        printf("\n Result:  This number is a prime number.\n\n");
    else
        printf("\n Result:  This number is not a prime number.\n\n");

    fclose(fp);

    return 0;
}
