#include <stdio.h>
#include <stdlib.h>

/* Copyright 2021 Melwyn Francis Carlo */

/* This value may be changed as required, but always append a '+1'. */
#define N 7E8+1

/*
    Note that this program takes up RAM space that is equal to 'N' times four, 
    that is, number of numerical elements times the size of an integer variable,
    which is four bytes (4B).

    In this case of 700 million numbers, 2.8GB RAM space will be occupied 
    throughout the duration of this program.
*/

int main()
{
    puts("\n File: Create_Primes_Test_1_to_700M_Numbers.C\n");
    puts(" ----------------------------------------");
    puts("-----------------------------------");
    puts("\n Copyright 2021 Melwyn Francis Carlo.\n\n");

    puts("\n Generating prime numbers test for natural numbers");
    puts(" upto seven hundred million (700M or 7E8).\n");
    puts(" This might take a while . . .");

    // Using the algorithm of Sieve of Eratosthenes
    int *marked = (int*)calloc(N+1, sizeof(int));
    for (int i = 2; (i*i) <= N; i++)
    {
        if (!marked[i])
        {
            for (int j = (i*i); j <= N; j += i)
                marked[j] = 1;
        }
    }
    marked[1] = 1;

    FILE *fp;

    fp = fopen("Prime_Numbers_Test_for_First_700_Million_Natural_Numbers", "w+");

    for (int i = 1; i < N; i++)
    {
        if (!marked[i])
            fprintf(fp, "1");
        else
            fprintf(fp, "0");
    }

    fprintf(fp, "\n");

    fclose(fp);

    free(marked);

    printf("\n Done.\n\n");

    return 0;
}
