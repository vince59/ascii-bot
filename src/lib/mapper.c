/*
** Ascii bot,
** mapper.c
** File description: mapper to create the maps
** 
*/

#include <stdlib.h>
#include <stdio.h>
#include "simulator.h"
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

t_cell **add_col(t_cell ** matrix, int col, int nb_col)
{
    return nb_col>0 ? realloc(matrix, sizeof(t_cell *) * (col+nb_col)) : matrix;
}

t_cell **add_row(t_cell ** matrix, int col, int row, int nb_row)
{
    int c;
    if (nb_row>0)
    for (c = 0; c < col; c++)
        matrix[c] = realloc(matrix[c],sizeof(t_cell) * (row + nb_row));

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

