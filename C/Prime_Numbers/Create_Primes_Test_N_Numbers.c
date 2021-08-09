#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Copyright 2021 Melwyn Francis Carlo */

#define MAX_NUM 1E6

/* These values may be changed as required. */
#define NUM_ARRAY_ELEMENTS 3
#define MAX_DIGITS 10

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
    int all_elements_equal = 1;

    for (int i = (NUM_ARRAY_ELEMENTS - 1); i >= 0; i--)
    {
        if ((num_array_1[i] > num_array_2[i]) && all_elements_equal)
            return 1;

        if (num_array_1[i] != num_array_2[i])
            all_elements_equal = 0;
    }

    if ((num_array_1[0] == num_array_2[0]) && all_elements_equal)
        return 0;

    return -1;
}

/*
Add-Num-Array: 
    Add all the elements and carry values of the two Num-Arrays
        onto Num-Array-1, that is,
        Num-Array-1[i] = Num-Array-1[i] + Num-Array-2[i] + Carry
*/
void add_num_array(int *num_array_1, const int *num_array_2)
{
    for (int i = 0; i < NUM_ARRAY_ELEMENTS; i++)
    {
        num_array_1[i] += num_array_2[i];

        if (num_array_1[i] >= MAX_NUM)
        {
            if (i == (NUM_ARRAY_ELEMENTS - 1))
            {
                printf("\n\n Error: The global constant parameter ");
                printf("'NUM_ARRAY_ELEMENTS' limit\n        has been exceeded.\n\n");
                exit(136);
            }

            while (num_array_1[i] >= MAX_NUM)
            {
                num_array_1[i] -= MAX_NUM;
                num_array_1[i+1]++;
            }
        }
    }
}

/* Set-Num-Array: Convert the character-based number into the Num-Array format. */
void set_num_array(int *num_array, char num[MAX_DIGITS+1])
{
    const int one[NUM_ARRAY_ELEMENTS] = { 1 };

    char count[MAX_DIGITS+1] = { 0 };
    memset(&count[0], '0', MAX_DIGITS);

    while (strcmp(count, num) < 0)
    {
        add_num_array(&num_array[0], &one[0]);
        increment(count);
    }
}

/* File-Seek-Increment: Increment the file pointer by a given offset. */
void file_seek_increment(FILE *file_pointer, int *file_position_offset, 
int *file_current_position, int *file_size)
{
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
    int i = 9;

    int count[NUM_ARRAY_ELEMENTS] = { 0 };

    int pos_i[NUM_ARRAY_ELEMENTS] = { 0 };

    while (compare_num_array(&count[0], &file_position_offset[0]) < 0)
    {
        int pre_count[NUM_ARRAY_ELEMENTS] = { 0 };
        pre_count[0] = power_refs[i];

        add_num_array(&pre_count[0], &count[0]);

        if (compare_num_array(&pre_count[0], &file_position_offset[0]) > 0)
        {
            if (i > 0) i--;
            continue;
        }

        memcpy(&pos_i[0], &file_current_position[0], sizeof(pos_i));
        add_num_array(&pos_i[0], &pre_count[0]);

        if (compare_num_array(&pos_i[0], &file_size[0]) >= 0)
            break;

        fseek(file_pointer, power_refs[i], SEEK_CUR);
        memcpy(&count[0], &pre_count[0], sizeof(count));
    }
}

int main()
{
    /* ---------------------------------------------------------------------- */
    /* Declare and initialize variables.                                      */

    /* ---------------------------------------------------------------------- */
        /* Set the maximum EVEN (NOT ODD) natural number over here. */
        char n_natural_numbers_str[MAX_DIGITS+1] = "1000";

        /*
        long int natural_number = 2E7;
        char n_natural_numbers_str[MAX_DIGITS+1] = { 0 };
        sprintf(&n_natural_numbers_str[0], "%ld", natural_number);
        */
    /* ---------------------------------------------------------------------- */

    printf("\n File: Create_Primes_Test_N_Numbers.C\n\n");
    printf(" Generating an ASCII-based boolean primes test for natural numbers\n");
    printf(" between 1 and %s.\n\n", n_natural_numbers_str);
    printf(" Copyright 2021 Melwyn Francis Carlo\n\n");
    printf(" ----------------------------------------");
    printf("-----------------------------------\n\n\n");

    pad(n_natural_numbers_str);

    char MAX_DIGITS_STR[11] = { 0 };
    sprintf(&MAX_DIGITS_STR[0], "%010d", MAX_DIGITS);

    char n_half_natural_numbers_str[MAX_DIGITS+1] = { 0 };
    memcpy(&n_half_natural_numbers_str[0], &n_natural_numbers_str[0], MAX_DIGITS);
    divideby2(n_half_natural_numbers_str);

    char current_n_str[MAX_DIGITS+1] = "3";
    pad(current_n_str);

    int n_natural_numbers[NUM_ARRAY_ELEMENTS] = { 0 };
    set_num_array(&n_natural_numbers[0], n_natural_numbers_str);

    int n_half_natural_numbers[NUM_ARRAY_ELEMENTS] = { 0 };
    set_num_array(&n_half_natural_numbers[0], n_half_natural_numbers_str);

    const int one[NUM_ARRAY_ELEMENTS] = { 1 };
    const int two[NUM_ARRAY_ELEMENTS] = { 2 };
    const int new[NUM_ARRAY_ELEMENTS] = { 9  + MAX_DIGITS };
    const int ten[NUM_ARRAY_ELEMENTS] = { 10 + MAX_DIGITS };

    int current_i[NUM_ARRAY_ELEMENTS] = { 0 };
    int current_n[NUM_ARRAY_ELEMENTS] = { 2 };

    int boolean = 1;

    FILE *fp;

    /* ---------------------------------------------------------------------- */
    /* Prepare file operation.                                                */

    printf(" Creating the 'Prime_Numbers_Test_of_First_N_Natural_Numbers' text file ...\n");

    fp = fopen("Prime_Numbers_Test_of_First_N_Natural_Numbers", "w+");

    printf(" Adding header content to the file ...\n");

    fprintf(fp, "%s%s", MAX_DIGITS_STR, n_natural_numbers_str);

    fprintf(fp, "01");

    printf(" Setting even numbers in the file contents to boolean false (0) ...\n\n");

    while (compare_num_array(&current_n[0], &n_natural_numbers[0]))
    {
        fprintf(fp, "%d", boolean);
        boolean = 1 - boolean;
        add_num_array(&current_n[0], &one[0]);
    }

    fprintf(fp, "\n");

    int file_size[NUM_ARRAY_ELEMENTS] = { 0 };
    set_num_array(&file_size[0], n_natural_numbers_str);
    add_num_array(&file_size[0], &ten[0]);

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

        fseek(fp, 10 + MAX_DIGITS, SEEK_SET);

        memset(&current_i[0], 0, sizeof(current_i));
        current_i[0] = 10 + MAX_DIGITS;
        file_seek_increment(fp, &current_n[0], &current_i[0], &file_size[0]);

        fseek(fp, -1, SEEK_CUR);

        memcpy(&current_i[0], &current_n[0], sizeof(current_i));
        add_num_array(&current_i[0], &new[0]);

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

    fclose(fp);

    printf("\n Done.\n\n");

    return 0;
}
