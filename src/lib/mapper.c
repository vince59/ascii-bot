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

// curr_r, curr_c : actual number of rows / colums
// nb_row, nb_col : number of row / columns you want to enlarge

t_cell **enlarge_map(t_cell **matrix, int curr_r, int curr_c, int nb_row, int nb_col)
{
    int i;

    matrix = realloc(matrix, sizeof(t_cell *) * (nb_col + curr_c));
    for (i = 0; i < (curr_c + nb_col); i++)
        matrix[i] = realloc(matrix[i], sizeof(t_cell) * (nb_row + curr_r));

    return matrix;
}

t_cell **add_rows(t_cell **matrix, int curr_r, int curr_c, int nb_row)
{
    if (nb_row <= 0)
        return matrix;
    matrix = enlarge_map(matrix, curr_r, curr_c, nb_row, 0);

    for (int l = curr_r; l < (curr_r + nb_row); l++)
    {
        for (int c = 0; c < curr_c; c++)
        {
            matrix[c][l].content = UNKNOWN;
        }
    }
    return matrix;
}

t_cell **add_cols(t_cell **matrix, int curr_r, int curr_c, int nb_col)
{
    if (nb_col <= 0)
        return matrix;
    matrix = enlarge_map(matrix, curr_r, curr_c, 0, nb_col);

    for (int c = curr_c; c < (curr_c + nb_col); c++)
    {
        for (int l = 0; l < curr_r; l++)
        {
            matrix[c][l].content = UNKNOWN;
        }
    }
    return matrix;
}

t_cell **insert_rows(t_cell **matrix, int curr_r, int curr_c, int nb_row)
{
    if (nb_row <= 0)
        return matrix;
    matrix = enlarge_map(matrix, curr_r, curr_c, nb_row, 0);
    for (int l = curr_r - 1; l >= 0; l--)
        for (int c = 0; c < curr_c; c++)
            matrix[c][l + nb_row].content = matrix[c][l].content;

    for (int l = 0; l < nb_row; l++)
        for (int c = 0; c < curr_c; c++)
            matrix[c][l].content = UNKNOWN;

    return matrix;
}

t_cell **insert_cols(t_cell **matrix, int curr_r, int curr_c, int nb_col)
{
    if (nb_col <= 0)
        return matrix;
    matrix = enlarge_map(matrix, curr_r, curr_c, 0, nb_col);
    for (int c = curr_c - 1; c >= 0; c--)
        for (int l = 0; l < curr_r; l++)
            matrix[c + nb_col][l].content = matrix[c][l].content;
    
    for (int c = 0; c < nb_col; c++)
        for (int l = 0; l < curr_r; l++)
            matrix[c][l].content = UNKNOWN;

    return matrix;
}

// display map to debug purpose
void display_map(t_cell **matrix, int row, int col)
{
    int l, c;

    for (l = 0; l < row; l++)
    {
        for (c = 0; c < col; c++)
            printf("%d ", matrix[c][l].content);
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
            matrix[c][l].content = FREE;
}
