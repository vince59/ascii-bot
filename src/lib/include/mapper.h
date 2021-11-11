/*
** Ascii bot,
** mapper.h
** File description: mapper to create the maps
** 
*/

#ifndef MAPPER_H
#define MAPPER_H

// obstacle type inside the matrix
#define ROBOT1   0
#define ROBOT2   1
#define ROBOT3   2
#define ROBOT4   3
#define ROBOT5   4

#define FREE     5
#define OBSTACLE 6


// orientation
#define N  0  // Nord
#define NE 1  // Nord Est
#define E  2  // Est
#define SE 3  // SudEst
#define S  4  // Sud
#define SO 5  // Sud Ouest
#define O  6  // Ouest
#define NO 7  // Nord Ouest

struct s_cell {
    int obstacle;
};

struct s_pos {
	int c,l;
	int orientation;
};

typedef struct s_cell t_cell; // cell of the matrix (map)
typedef struct s_pos t_pos;   // position inside the matrix

t_cell  **gen_map(int, int);     
void    display_map(t_cell **, int); // display map to debug purpose
void    init_map(t_cell **, int, int);    // init a exiting map
#endif


