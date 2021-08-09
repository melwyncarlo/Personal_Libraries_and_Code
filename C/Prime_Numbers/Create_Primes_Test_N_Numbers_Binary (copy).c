#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Copyright 2021 Melwyn Francis Carlo */

#define MAX_NUM 1E6

/* These values may be changed as required. */
#define NUM_ARRAY_ELEMENTS 3
#define MAX_DIGITS 10

/* Increment: Num = Num + 1 */
void increment(char num[MAX_DIGITS+1])
{
    num[MAX_DIGITS-1]++;

    if (num[MAX_DIGITS-1] <= '9') return;

    int i = MAX_DIGITS - 1;
    while (num[i] > '9')
    {
        if (i == 0)
        {
            printf("\n\n Error: The global constant parameter 'MAX_DIGITS' ");
            printf("limit\n        has been exceeded.\n\n");
            exit(136);
        }
        num[i]    = '0';
        num[i-1] += 1;
        i--;
    }
}

/* Divide-By-32: Num = Num / 32, and Return = Num Modulo 32. */
int divideby32(char num[MAX_DIGITS+1])
{
    int temp_val  = 0;
    int carry_val = 0;

    for (int i = 0; i < MAX_DIGITS; i++)
    {
        temp_val  = num[i] - '0';
        num[i]    = ((temp_val + (carry_val * 10)) / 32) + '0';
        carry_val =  (temp_val + (carry_val * 10)) % 32;
    }

    return carry_val;
}

/* Divide-By-2: Num = Num / 2 */
void divideby2(char num[MAX_DIGITS+1])
{
    int temp_val  = 0;
    int carry_val = 0;

    for (int i = 0; i < MAX_DIGITS; i++)
    {
        temp_val  = num[i] - '0';
        num[i]    = ((temp_val / 2) + carry_val) + '0';
        carry_val = (temp_val % 2) * 5;
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

/*
Compare-Num-Array: 
   Return = -1, if Num-Array-1 < Num-Array-2
   Return =  0, if Num-Array-1 = Num-Array-2
   Return = +1, if Num-Array-1 > Num-Array-2
*/
int compare_num_array(int *num_array_1, int *num_array_2)
{
    for (int i = (NUM_ARRAY_ELEMENTS - 1); i >= 0; i--)
        if (num_array_1[i] > num_array_2[i])
            return 1;

    if (num_array_1[0] == num_array_2[0])
        return 0;

    return -1;
}

/*
Increment-Num-Array: 
   Increment the first element of the Num-Array by one, that is,
      Num-Array[0] = Num-Array[0] + 1.
   If a carry value exists, then add them onto the 
      remaining elements of the Num-Array.
*/
void increment_num_array(int *num_array)
{
    num_array[0]++;

    if (num_array[0] <= MAX_NUM) return;

    int i = 0;
    while (num_array[i] > MAX_NUM)
    {
        if (i == (NUM_ARRAY_ELEMENTS - 1))
        {
            printf("\n\n Error: The global constant parameter ");
            printf("'NUM_ARRAY_ELEMENTS' limit\n        has been exceeded.\n\n");
            exit(136);
        }
        num_array[i] = 0;
        num_array[++i]++;
    }
}

/* Set-Num-Array: Convert the character-based number into the Num-Array format. */
void set_num_array(int *num_array, char num[MAX_DIGITS+1])
{
    char count[MAX_DIGITS+1] = { 0 };
    memset(&count[0], '0', MAX_DIGITS);

    while (strcmp(count, num) < 0)
    {
        increment_num_array(&num_array[0]);
        increment(count);
    }
}

/* File-Seek-Increment: Increment the file pointer by a given offset. */
void file_seek_increment(FILE *file_pointer, int *file_position, int *file_size)
{
    int count[NUM_ARRAY_ELEMENTS] = { 0 };
    count[0] = 0;

    while ((compare_num_array(&count[0], &file_position[0])) 
    &&     (ftell(file_pointer) <= file_size[0]))
    {
        fseek(file_pointer, 1, SEEK_CUR);
        increment_num_array(&count[0]);
    }
}

int main()
{
    /* ---------------------------------------------------------------------- */
    /* Declare and initialize variables.                                      */

    /* ---------------------------------------------------------------------- */
        /* Set the maximum EVEN (NOT ODD) natural number over here. */
        char n_natural_numbers_str[MAX_DIGITS+1] = "100";

        /*
        long int natural_number = 7E8;
        char n_natural_numbers_str[MAX_DIGITS+1];
        sprintf(&n_natural_numbers_str[0], "%ld", natural_number);
        */
    /* ---------------------------------------------------------------------- */

    printf("\n\n");
    printf(" Generating a binary-based boolean primes test for natural numbers\n");
    printf(" between 1 and %s.\n\n", n_natural_numbers_str);
    printf(" Copyright 2021 Melwyn Francis Carlo\n\n");
    printf(" ----------------------------------------");
    printf("-----------------------------------\n\n\n");

    pad(n_natural_numbers_str);

    char n_half_natural_numbers_str[MAX_DIGITS+1] = { 0 };
    memcpy(&n_half_natural_numbers_str[0], &n_natural_numbers_str[0], MAX_DIGITS);
    divideby2(n_half_natural_numbers_str);

    int n_natural_numbers[NUM_ARRAY_ELEMENTS] = { 0 };
    set_num_array(&n_natural_numbers[0], n_natural_numbers_str);

    int n_half_natural_numbers[NUM_ARRAY_ELEMENTS] = { 0 };
    set_num_array(&n_half_natural_numbers[0], n_half_natural_numbers_str);

    int current_n[NUM_ARRAY_ELEMENTS] = { 0 };
    current_n[0] = 2;

    char current_n_str[MAX_DIGITS+1] = "3";
    pad(current_n_str);

    int modulo32;

    char n_doublewords[MAX_DIGITS+1] = { 0 };
    memcpy(&n_doublewords[0], &n_natural_numbers_str[0], MAX_DIGITS);
    modulo32 = divideby32(n_doublewords);
    if (modulo32) increment(n_doublewords);

    char doubleword_i[MAX_DIGITS+1] = { 0 };
    memset(&doubleword_i[0], '0', MAX_DIGITS);
    doubleword_i[MAX_DIGITS-1] = '1';

    int max_digits_in_32s = MAX_DIGITS / 32;
    if (MAX_DIGITS % 32) max_digits_in_32s++;

    char n_natural_numbers_str_in_32s[(max_digits_in_32s*32)+1];
    memset(&n_natural_numbers_str_in_32s[0], 0, (max_digits_in_32s*32) + 1);
    memcpy(&n_natural_numbers_str_in_32s[0], &n_natural_numbers_str[0], MAX_DIGITS);
    pad(n_natural_numbers_str_in_32s);

    uint32_t n_natural_numbers_in_32s[max_digits_in_32s];

    for (int i = 0; i < max_digits_in_32s; i++)
    {
        char temp_str[33] = { 0 };
        memcpy(&temp_str[0], &n_natural_numbers_str_in_32s[i*32], 32);

        n_natural_numbers_in_32s[i] = (uint32_t)atoi(temp_str);
    }

    const uint32_t MAX_BYTES_UINT32 = (uint32_t)max_digits_in_32s;

    /* 1789569706d = 0b01101010101010101010101010101010 */
    const uint32_t FIRST_DOUBLEWORD_DATA = 1789569706;
    /* 2863311530d = 0b10101010101010101010101010101010 */
    const uint32_t OTHER_DOUBLEWORD_DATA = 2863311530;

    FILE *fp;

    /* ---------------------------------------------------------------------- */
    /* Prepare file operation.                                                */

    printf(" Creating the 'Prime_Numbers_of_First_N_Natural_Numbers' text file ...\n");

    fp = fopen("Prime_Numbers_of_First_N_Natural_Numbers.bin", "w+b");

    printf(" Adding header content to the file ...\n");

    fwrite(&MAX_BYTES_UINT32, sizeof(uint32_t), 1, fp);

    for (int i = 0; i < max_digits_in_32s; i++)
        fwrite(&n_natural_numbers_in_32s[i], sizeof(uint32_t), 1, fp);

    fwrite(&FIRST_DOUBLEWORD_DATA, sizeof(uint32_t), 1, fp);

    printf(" Setting even numbers in the file contents to boolean false (0) ...\n\n");

    while (strcmp(&doubleword_i[0], &n_doublewords[0]))
    {
        fwrite(&OTHER_DOUBLEWORD_DATA, sizeof(uint32_t), 1, fp);
        increment(doubleword_i);
    }

    //fclose(fp);

    /* ---------------------------------------------------------------------- */

    printf(" Setting multiples of odd numbers (but not the number itself)\n");
    printf(" in the file contents to boolean false (0).\n\n");

    printf(" From 3 to (%s - 1) ...\n\n", n_half_natural_numbers_str);

    memset(&current_n[0], 0, sizeof(current_n));
    current_n[0] = 3;

    while (compare_num_array(&current_n[0], &n_half_natural_numbers[0]) == -1)
    {
        printf(" # %s ...\n", current_n_str);
        increment(current_n_str);
        increment(current_n_str);

        fseek(fp, max_digits_in_32s, SEEK_SET);

        //memset(&current_i[0], 0, sizeof(current_i));
        //current_i[0] = 10 + MAX_DIGITS;
        //file_seek_increment(fp, &current_n[0], &current_i[0], &file_size[0]);

        //fseek(fp, -1, SEEK_CUR);

        //memcpy(&current_i[0], &current_n[0], sizeof(current_i));
        //add_num_array(&current_i[0], &new[0]);

        memcpy(&doubleword_i[0], &current_n_str[0], sizeof(current_n_str));
        modulo32 = divideby32(doubleword_i);
        increment(doubleword_i);

        while (1)
        {
            file_seek_increment(fp, &current_n[0], &current_i[0], &file_size[0]);
            add_num_array(&current_i[0], &current_n[0]);

            if (compare_num_array(&current_i[0], &file_size[0]) >= 0) break;

            fprintf(fp, "0");
            fseek(fp, -1, SEEK_CUR);
        }

        add_num_array(&current_n[0], &two[0]);
    }

    while (compare_num_array(&current_n[0], &n_half_natural_numbers[0]) == -1)
    {
        fseek(fp, 10 + MAX_DIGITS, SEEK_SET);

        file_seek_increment(fp, &current_n[0], &n_natural_numbers[0]);

        fseek(fp, -1, SEEK_CUR);

        while (1)
        {
            file_seek_increment(fp, &current_n[0], &n_natural_numbers[0]);

            if (ftell(fp) >= n_natural_numbers[0]) break;

            fprintf(fp, "0");

            fseek(fp, -1, SEEK_CUR);
        }

        increment_num_array(&current_n[0]);
        increment_num_array(&current_n[0]);
    }

    fclose(fp);

    printf("\n Done.\n\n");

    return 0;
}
