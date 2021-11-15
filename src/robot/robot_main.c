/*
** Ascii bot, 2021
** main.c
** File description: robot main file
** 
*/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>

#include "network.h"
#include "utils.h"
#include "mapper.h"
#include "basic_cmd.h"
#include "robot.h"
#include "simulator.h"

t_map map;
int move_to(int direction, int *status, int sim_socket);

int main(int argc, char *argv[])
{

map.l=1;
map.c=1;

map.map = gen_map(map.l, map.c);

map.map[map.l-1][map.c-1].obstacle=ROBOT1;

display_map(map.map, map.l, map.c);

map.map=add_col(map.map, map.c, 3);


map.c+=3;
puts("****\n");
display_map(map.map, map.l, map.c);

map.map=add_row(map.map, map.c, map.l, 2);

map.l+=2;

display_map(map.map, map.l, map.c);



	return EXIT_SUCCESS;
	char sim_srv[40], mapper_srv[40];
	int sim_port, mapper_port, sim_socket, mapper_socket = 0, id;

	if (get_bot_param(argc, argv, sim_srv, &sim_port, mapper_srv, &mapper_port))
		return EXIT_FAILURE;
	if (mapper_port > 0)
	{
		if (open_comm(mapper_srv, mapper_port, &mapper_socket))
			return EXIT_FAILURE;
	}
	if (open_comm(sim_srv, sim_port, &sim_socket))
		return EXIT_FAILURE;
	if (get_id(&id, sim_socket))
		return EXIT_FAILURE;
	printf("I'm robot #%d, and i'm in the map !\n", id);

	test_find_target(sim_socket, mapper_socket);

	puts("Bye !\n");
	close_comm(sim_socket);
	if (mapper_socket > 0)
		close_comm(mapper_socket);
	return EXIT_SUCCESS;
}

int test_find_target(int sim_socket, int mapper_socket)
{
	map.max_c = map.max_l = map.l = map.c = 1;
	map.map = gen_map(map.max_l, map.max_c);
	map.mapper_socket = mapper_socket;

	if (set_cell(map.max_c, map.max_l, ROBOT1, map.mapper_socket))
		return EXIT_FAILURE;

	int dir, dist, info, status;
	do
	{
		if (find_target(&dir, &dist, sim_socket))
			return EXIT_FAILURE;
		if (dir > -1)
		{
			printf("Target localized at %d %d\n", dir, dist);
			do
			{
				if (go_to(dir, &status, sim_socket))
					return EXIT_FAILURE;
			} while (status == CMD_OK);
			puts("Target catched !\n");
			return EXIT_SUCCESS;
		}
		for (int d = 0; d < 8; d++)
		{
			if (scan(d, &dist, &info, sim_socket))
				return EXIT_FAILURE;
			if (info == 0 || (info == OBSTACLE && dist > 2))
			{
				dir = d;
				break;
			}
		}
		if (go_to(dir, &status, sim_socket))
			return EXIT_FAILURE;
	} while (1);
	return EXIT_SUCCESS;
}

int move_to(int direction, int *status, int sim_socket)
{

	if (go_to(direction, status, sim_socket))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}