#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Copyright 2021 Melwyn Francis Carlo */

int main()
{
    int sudoku[9][9] = { 0 };

    printf("\n File: Sudoku_Solver.C\n");
    printf(" ----------------------------------------");
    printf("-----------------------------------");
    printf("\n Copyright 2021 Melwyn Francis Carlo.\n\n");

    printf("\n How do you wish to enter your Sudoku question?\n");
    printf("\n 1. Cell-wise");
    printf("\n 2. Row-wise\n");

    printf("\n Enter 1 or 2 :  ");

    int input_choice;

    if (scanf("%d", &input_choice) == -1)
    {
        printf("\n\n Error: An unknown I/O error has occurred. Please report.\n\n");
        exit(1);
    }

    if ((input_choice < 1) || (input_choice > 2))
    {
        printf("\n\n Error: The input choice entered is invalid.\n\n");
        exit(1);
    }

    printf("\n\n");

    /* ---------------------------------------------------------------------- */
    /* This is a cell-wise input of the Sudoku question.                      */
    if (input_choice == 1)
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                printf(" Enter the data for Row-%d | Column-%d :  ", i + 1, j + 1);

                if (scanf("%d", &sudoku[i][j]) == -1)
                {
                    printf("\n\n Error: An unknown I/O error has occurred. Please report.\n\n");
                    exit(1);
                }
            }
        }
    }
    /* ---------------------------------------------------------------------- */
    /* This is a row-wise input of the Sudoku question.                       */
    else /* if (input_choice == 2) */
    {
        char row_data[10] = { 0 };

        for (int i = 0; i < 9; i++)
        {
            printf(" Enter the data for Row-%d :  ", i + 1);

            if (scanf("%s", row_data) == -1)
            {
                printf("\n\n Error: An unknown I/O error has occurred. Please report.\n\n");
                exit(1);
            }

            for (int j = 0; j < 9; j++)
                sudoku[i][j] = row_data[j] - '0';
        }
    }

    /* ---------------------------------------------------------------------- */

    printf("\n\n The Sudoku question is as follows:\n ");
    for (int i = 0; i < 9; i++)
    {
        if ((i % 3) == 0)
        {
            if (i == 0)
            {
                printf(" %s\n %s\n ", "___________________________________", 
                                      "|           |           |           |");
            }
            else
            {
                printf("%s\n %s\n ", "|___________|___________|___________|", 
                                     "|           |           |           |");
            }
        }

        for (int j = 0; j < 9; j++)
        {
            if ((j % 3) == 0)
                printf("|  ");

            if (sudoku[i][j])
                printf("%d  ", sudoku[i][j]);
            else
                printf("-  ");
        }
        printf("|\n ");
    }
    printf("%s\n ", "|___________|___________|___________|");

    /* ---------------------------------------------------------------------- */

    int is_fixed_cell[9][9] = { 0 };

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (sudoku[i][j])
                is_fixed_cell[i][j] = 1;

    int digit[9][9][10] = { 0 };

    int i = 0;
    while (i < 9)
    {
        int j = 0;
        while (j < 9)
        {
            if (is_fixed_cell[i][j])
            {
                j++;
                continue;
            }

            int i2 = (int)(i / 3) * 3;
            int j2 = (int)(j / 3) * 3;

            int duplicate_found = 1;

            if (!digit[i][j][1] || !digit[i][j][2] || !digit[i][j][3] 
            ||  !digit[i][j][4] || !digit[i][j][5] || !digit[i][j][6] 
            ||  !digit[i][j][7] || !digit[i][j][8] || !digit[i][j][9])
            {
                for (int k = 1; k <= 9; k++)
                {
                    if (digit[i][j][k])
                        continue;

                    duplicate_found = 0;

                    for (int l = 0; l < 9; l++)
                    {
                        if (l != i)
                        {
                            if (sudoku[l][j] == k)
                            {
                                duplicate_found = 1;
                                break;
                            }
                        }

                        if (l != j)
                        {
                            if (sudoku[i][l] == k)
                            {
                                duplicate_found = 1;
                                break;
                            }
                        }
                    }

                    if (!duplicate_found)
                    {
                        if ((sudoku[i2][j2]     == k) || (sudoku[i2][j2+1]   == k) 
                        ||  (sudoku[i2][j2+2]   == k) || (sudoku[i2+1][j2]   == k) 
                        ||  (sudoku[i2+1][j2+1] == k) || (sudoku[i2+1][j2+2] == k) 
                        ||  (sudoku[i2+2][j2]   == k) || (sudoku[i2+2][j2+1] == k) 
                        ||  (sudoku[i2+2][j2+2] == k)) 
                            duplicate_found = 1;
                    }

                    if (!duplicate_found)
                    {
                        digit[i][j][k] = 1;
                        sudoku[i][j]   = k;
                        break;
                    }
                }
            }

            if (duplicate_found)
            {
                memset(&digit[i][j][0], 0, 10 * sizeof(int));

                sudoku[i][j] = 0;

                j--;
                if (j < 0)
                {
                    j = 8;
                    i--;
                }
                while (is_fixed_cell[i][j])
                {
                    j--;
                    if (j < 0)
                    {
                        j = 8;
                        i--;
                    }
                }
            }
            else
            {
                j++;
            }
        }

        i++;
    }

    /* ---------------------------------------------------------------------- */

    printf("\n\n The Sudoku solution is as follows:\n ");
    for (int i = 0; i < 9; i++)
    {
        if ((i % 3) == 0)
        {
            if (i == 0)
            {
                printf(" %s\n %s\n ", "___________________________________", 
                                      "|           |           |           |");
            }
            else
            {
                printf("%s\n %s\n ", "|___________|___________|___________|", 
                                     "|           |           |           |");
            }
        }

        for (int j = 0; j < 9; j++)
        {
            if ((j % 3) == 0)
                printf("|  ");

            if (sudoku[i][j])
                printf("%d  ", sudoku[i][j]);
            else
                printf("-  ");
        }
        printf("|\n ");
    }
    printf("%s\n\n\n", "|___________|___________|___________|");

    return 0;
}
