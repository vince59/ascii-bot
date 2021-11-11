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

