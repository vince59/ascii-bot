/*
** Ascii bot, 2021
** simulator.h
** Useful functions
*/

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "mapper.h"

#define MAX_ROBOT 5

struct s_map
{
    t_cell **map;
    t_pos robots[MAX_ROBOT];
    int nb_robots, max_l, max_c, mapper_socket, max_robots;
};

typedef struct s_map t_map;

#endif