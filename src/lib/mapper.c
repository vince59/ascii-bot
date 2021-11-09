/*
** SIMBOT PROJECT, 2020
** mapper.c
** File description: mapper to create the maps
** 
*/

#include <stdlib.h>
#include <stdio.h>
#include "mapper.h"

// allocate memory to a new map
t_cell_struct **gen_map(int row, int col)
{
    t_cell_struct **matrix;
    int c;

    matrix = malloc(sizeof(t_cell_struct *) * col);
    for (c = 0; c < col; c++)
        matrix[c] = malloc(sizeof(t_cell_struct) * row);

    return matrix;
}

// display map to debug purpose
void display_map(t_cell_struct **matrix, int size)
{
    int l, c;

    for (l = 0; l < size; l++)
    {
        for (c = 0; c < size; c++)
            printf("%d ", matrix[c][l].obstacle);
        printf("\n");
    }
    printf("\n\n");
}

// init a exiting map
void init_map(t_cell_struct **matrix, int row, int col)
{
    int l, c;

    for (l = 0; l < row; l++)
    {
        for (c = 0; c < col; c++)
        {
            matrix[c][l].obstacle = (c==0 || l==0 || c== col-1 || l==row-1) ? OBSTACLE : FREE;
        }
    }

    matrix[4][7].obstacle = ROBOT;
}
