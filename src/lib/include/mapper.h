/*
** SIMBOT PROJECT, 2020
** mapper.h
** File description: mapper to create the maps
** 
*/

#ifndef MAPPER_H
#define MAPPER_H

// obstacle type inside the matrix
#define FREE     0
#define OBSTACLE 1
#define ROBOT    2

// orientation
#define N  0  // Nord
#define NE 1  // Nord Est
#define E  2  // Est
#define SE 3  // SudEst
#define S  4  // Sud
#define SO 5  // Sud Ouest
#define O  6  // Ouest
#define NO 7  // Nord Ouest

struct s_cell_struct {
    int obstacle;
};

struct s_pos_struct {
	int x,y;
	int orientation;
};

typedef struct s_cell_struct t_cell_struct; // cell of the matrix (map)
typedef struct s_pos_struct t_pos_struct;   // position inside the matrix

t_cell_struct  **gen_map(int, int);     
void           display_map(t_cell_struct **, int); // display map to debug purpose
void           init_map(t_cell_struct **, int, int);    // init a exiting map
#endif


