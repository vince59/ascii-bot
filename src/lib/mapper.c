/*
** Ascii bot,
** mapper.c
** File description: mapper to create the maps
** 
*/

#include <stdlib.h>
#include <stdio.h>
#include "mapper.h"

// allocate memory to a new map
t_cell **gen_map(int row, int col)
{
    t_cell **matrix;
    int c;

    matrix = malloc(sizeof(t_cell *) * col);
    for (c = 0; c < col; c++)
        matrix[c] = malloc(sizeof(t_cell) * row);

    return matrix;
}

// display map to debug purpose
void display_map(t_cell **matrix, int row, int col)
{
    int l, c;

    for (l = 0; l < row; l++)
    {
        for (c = 0; c < col; c++)
            printf("%d ", matrix[c][l].obstacle);
        printf("\n");
    }
    printf("\n\n");
}

// init a exiting map
void init_map(t_cell **matrix, int row, int col)
{
    int l, c;

    for (l = 0; l < row; l++) 
        for (c = 0; c < col; c++)
            matrix[c][l].obstacle = FREE;
}

t_cell ** load_map(char *file_name, int *rows, int *cols, int *bots)
{
    int c;
    FILE *file;
    file = fopen(file_name, "r");
    *rows = 0;
    *cols = 0;
    *bots = 0;
    t_cell ** map;
    if (file)
    {
        while ((c = getc(file)) != EOF)
        {
            if (c == '\n')
            {
                (*rows)++;
            }
            else
            {
                if (*rows == 0)
                    (*cols)++;
                if (c != ' ' && c != 'X')
                    (*bots)++;
            }
        }
        printf("Max rows = %d, max cols = %d, max bots = %d\n", *rows, *cols, *bots);
        map = gen_map(*rows, *cols);
	    init_map(map, *rows, *cols);
        
        rewind(file);
        int col=0, row=0;

        while ((c = getc(file)) != EOF)
        {
            switch (c) 
            {
                case '\n' : 
                    col=0;
                    row++;
                    break;
                case 'X' :
                    map[col++][row].obstacle = OBSTACLE;
                    break;
                case '0' :
                    map[col++][row].obstacle = 0;
                    break;
                case '1' :
                    map[col++][row].obstacle = 1;
                    break;
                default :
                    col++;

            }
        }

        fclose(file);
    }
    else
    {
        puts("Error when loding map, check map file path\n");
        return NULL;
    }

    return map;
}