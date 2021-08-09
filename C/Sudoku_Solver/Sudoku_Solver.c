#include <stdio.h>
#include <string.h>

/* Copyright 2021 Melwyn Francis Carlo */

int main()
{
    int is_fixed_cell[9][9] = { 0 };

    /* ---------------------------------------------------------------------- */
    /* Enter your Sudoku question over here. Denote blanks with zeroes.       */

    int sudoku[9][9] = {
                            {  0,  0,  0,  0,  0,  0,  2,  0,  0  }, 
                            {  0,  8,  0,  0,  0,  7,  0,  9,  0  }, 
                            {  6,  0,  2,  0,  0,  0,  5,  0,  0  }, 
                            {  0,  7,  0,  0,  6,  0,  0,  0,  0  }, 
                            {  0,  0,  0,  9,  0,  1,  0,  0,  0  }, 
                            {  0,  0,  0,  0,  2,  0,  0,  4,  0  }, 
                            {  0,  0,  5,  0,  0,  0,  6,  0,  3  }, 
                            {  0,  9,  0,  4,  0,  0,  0,  7,  0  }, 
                            {  0,  0,  6,  0,  0,  0,  0,  0,  0  } 
                       };

    /* ---------------------------------------------------------------------- */
    /* This is a cell-wise input of the Sudoku question.                      */

    /*
        int sudoku[9][9] = { 0 };

        printf("\n");
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                printf(" Enter the data for Row-%d | Column-%d :  ", i + 1, j + 1);
                scanf("%d", &sudoku[i][j]);
            }
        }
    */

    /* ---------------------------------------------------------------------- */
    /* This is a row-wise input of the Sudoku question.                       */

    /*
        int sudoku[9][9]  = { 0 };

        char row_data[10] = { 0 };

        printf("\n");
        for (int i = 0; i < 9; i++)
        {
            printf(" Enter the data for Row-%d :  ", i + 1);
            scanf("%s", row_data);

            for (int j = 0; j < 9; j++)
                sudoku[i][j] = row_data[j] - '0';
        }
    */

    /* ---------------------------------------------------------------------- */

    printf("\n File: Sudoku_Solver.C\n");
    printf(" ----------------------------------------");
    printf("-----------------------------------");
    printf("\n Copyright 2021 Melwyn Francis Carlo.\n\n");

    printf("\n The Sudoku question is as follows:\n ");
    for (int i = 0; i < 9; i++)
    {
        if ((i % 3) == 0)
            printf("%s\n ", "—————————————————————————————————————");

        for (int j = 0; j < 9; j++)
        {
            if ((j % 3) == 0)
                printf("|  ");

            if (sudoku[i][j])
                printf("%d  ", sudoku[i][j]);
            else
                printf("∙  ");
        }
        printf("|\n ");
    }
    printf("%s\n ", "—————————————————————————————————————");

    /* ---------------------------------------------------------------------- */

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
            printf("%s\n ", "—————————————————————————————————————");

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
    printf("%s\n\n\n", "—————————————————————————————————————");

    return 0;
}
