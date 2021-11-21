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
int test_matrix();
int update_map();

int main(int argc, char *argv[])
{
	//return test_matrix();

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

int test_matrix()
{
	map.max_c = 4;
	map.max_l = 3;
	map.l = map.c = 0;
	map.map = gen_map(map.max_l, map.max_c);
	init_map(map.map, map.max_l, map.max_c);

	map.map[map.c][map.l].content = 3;
	display_map(map.map, map.max_l, map.max_c);

	puts("Ajout 2 lignes\n");
	map.map = add_rows(map.map, map.max_l, map.max_c, 2);
	map.max_l += 2;
	display_map(map.map, map.max_l, map.max_c);

	puts("Ajout 1 colonne\n");
	map.map = add_cols(map.map, map.max_l, map.max_c, 1);
	map.max_c++;
	display_map(map.map, map.max_l, map.max_c);

	puts("Insertion 2 lignes\n");
	map.map = insert_rows(map.map, map.max_l, map.max_c, 2);
	map.max_l += 2;
	display_map(map.map, map.max_l, map.max_c);

	puts("Insertion 2 colonne\n");
	map.map = insert_cols(map.map, map.max_l, map.max_c, 2);
	map.max_c += 2;
	display_map(map.map, map.max_l, map.max_c);

	return EXIT_SUCCESS;
}

int update_map()
{
	for (int l = 0; l < map.max_l; l++)
		for (int c = 0; c < map.max_c; c++)
			if (set_cell(c, l, map.map[c][l].content, map.mapper_socket))
				return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int test_find_target(int sim_socket, int mapper_socket)
{
	map.max_c = map.max_l = 1;
	map.l = map.c = 0;
	map.map = gen_map(map.max_l, map.max_c);

	map.map[map.c][map.l].content = ROBOT1;
	map.mapper_socket = mapper_socket;

	int dist, info;

	if (update_map())
		return EXIT_FAILURE;

	for (int d = 0; d < 8; d++)
	{
		if (scan(d, &dist, &info, sim_socket))
			return EXIT_FAILURE;
		int l,c;
		switch (d)
		{
		case N:
			l = map.l - dist;
			if (l < 0)
			{
				l = -l;
				map.map = insert_rows(map.map, map.max_l, map.max_c, l);
				map.max_l += l;
				map.l += l;
			}
			for (int i = 1; i < dist; i++)
				map.map[map.c][map.l - i].content = FREE;
			map.map[map.c][map.l - dist].content = info;
			break;
		case NE:
			break;
		case E:
			break;
		case SE:
			break;
		case S:
			l = map.l + 1 + dist;
			if (l > map.max_l)
			{
				l = l - map.max_l;

				map.map = add_rows(map.map, map.max_l, map.max_c, l);
				map.max_l += l;
			}
			for (int i = 1; i < dist; i++)
				map.map[map.c][map.l + i].content = FREE;
			map.map[map.c][map.l + dist].content = info;
			break;
		case SO:
			break;
		case O:
			printf("%d %d\n", dist, info);
			c = map.c - dist;
			if (c < 0)
			{
				c = -c;
				map.map = insert_cols(map.map, map.max_l, map.max_c, c);
				map.max_c += c;
				map.c += c;
			}
			for (int i = 1; i < dist; i++)
				map.map[map.c-i][map.l].content = FREE;
			map.map[map.c-dist][map.l].content = info;
			display_map(map.map, map.max_l, map.max_c);
			break;
		case NO:
			break;
		default:
			return EXIT_FAILURE;
		}
	}

	update_map();

	char s[30];
	scanf("%s", s);
	return EXIT_SUCCESS;

	int l = map.l - dist;
	int nb_l;
	if (l < 0)
	{
		nb_l = -l;
	}

	if (l > 0)
	{
		map.map = enlarge_map(map.map, map.max_l, map.max_c, nb_l, 0);

		for (int i = nb_l; i > 0; i--)
		{
			map.map[0][i].content = map.map[0][i - 1].content;
		}

		map.max_l += nb_l;

		if (set_cell(map.c, map.l, ROBOT1, map.mapper_socket))
			return EXIT_FAILURE;
	}

	printf(">> %d %d\n", dist, info);
	printf(">> %d\n", nb_l);

	return EXIT_SUCCESS;

	/*
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

		if (scan(N, &dist, &info, sim_socket))
				return EXIT_FAILURE;

		for (int d = 0; d < 8; d++)
		{
			if (scan(d, &dist, &info, sim_socket))
				return EXIT_FAILURE;
			if (info == 0 || (info == content && dist > 2))
			{
				dir = d;
				break;
			}
		}
		if (go_to(dir, &status, sim_socket))
			return EXIT_FAILURE;
			break;
	} while (1);*/
	return EXIT_SUCCESS;
}

int move_to(int direction, int *status, int sim_socket)
{

	if (go_to(direction, status, sim_socket))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}