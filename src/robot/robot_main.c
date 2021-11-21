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

int update_debug_map()
{
	for (int l = 0; l < map.max_l; l++)
		for (int c = 0; c < map.max_c; c++)
			if (set_cell(c, l, map.map[c][l].content, map.mapper_socket))
				return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

void update_map_N(int dist)
{
	int l = map.l - dist;
	if (l < 0)
	{
		l = -l;
		map.map = insert_rows(map.map, map.max_l, map.max_c, l);
		map.max_l += l;
		map.l += l;
	}
}

void update_map_E(int dist)
{
	int c = map.c + 1 + dist;
	if (c > map.max_c)
	{
		c = c - map.max_c;
		map.map = add_cols(map.map, map.max_l, map.max_c, c);
		map.max_c += c;
	}
}

void update_map_S(int dist)
{
	int l = map.l + 1 + dist;
	if (l > map.max_l)
	{
		l = l - map.max_l;
		map.map = add_rows(map.map, map.max_l, map.max_c, l);
		map.max_l += l;
	}
}

void update_map_O(int dist)
{
	int c = map.c - dist;
	if (c < 0)
	{
		c = -c;
		map.map = insert_cols(map.map, map.max_l, map.max_c, c);
		map.max_c += c;
		map.c += c;
	}
}

int update_map(int sim_socket)
{
	int dist, info;
	for (int d = 0; d < 8; d++)
	{
		if (scan(d, &dist, &info, sim_socket))
			return EXIT_FAILURE;
		switch (d)
		{
		case N:
			update_map_N(dist);
			for (int i = 1; i < dist; i++)
				map.map[map.c][map.l - i].content = FREE;
			map.map[map.c][map.l - dist].content = info;
			break;
		case NE:
			update_map_N(dist);
			update_map_E(dist);
			for (int i = 1; i < dist; i++)
				map.map[map.c + i][map.l - i].content = FREE;
			map.map[map.c + dist][map.l - dist].content = info;
			break;
		case E:
			update_map_E(dist);
			for (int i = 1; i < dist; i++)
				map.map[map.c + i][map.l].content = FREE;
			map.map[map.c + dist][map.l].content = info;
			break;
		case SE:
			update_map_S(dist);
			update_map_E(dist);
			for (int i = 1; i < dist; i++)
				map.map[map.c + i][map.l + i].content = FREE;
			map.map[map.c + dist][map.l + dist].content = info;
			break;
		case S:
			update_map_S(dist);
			for (int i = 1; i < dist; i++)
				map.map[map.c][map.l + i].content = FREE;
			map.map[map.c][map.l + dist].content = info;
			break;
		case SO:
			update_map_S(dist);
			update_map_O(dist);
			for (int i = 1; i < dist; i++)
				map.map[map.c - i][map.l + i].content = FREE;
			map.map[map.c - dist][map.l + dist].content = info;
			break;
		case O:
			update_map_O(dist);
			for (int i = 1; i < dist; i++)
				map.map[map.c - i][map.l].content = FREE;
			map.map[map.c - dist][map.l].content = info;
			break;
		case NO:
			update_map_N(dist);
			update_map_O(dist);
			for (int i = 1; i < dist; i++)
				map.map[map.c - i][map.l - i].content = FREE;
			map.map[map.c - dist][map.l - dist].content = info;
			break;
		default:
			return EXIT_FAILURE;
		}
	}
	if (update_debug_map())
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int find_uncleared_zone(int *nb_l, int *nb_c)
{
	int j = map.max_l > map.max_c ? map.max_l : map.max_c;
	int l, c;

	*nb_l = *nb_c = 0;
	for (int i = 1; i < j; i++)
	{
		l = map.l + i;
		c = map.c + i;
		if (c < map.max_c && l < map.max_l)
			if (map.map[c][l].content == UNKNOWN)
			{
				*nb_l = i;
				*nb_c = i;
				break;
			}

		l = map.l - i;
		c = map.c - i;
		if (c > 0 && l > 0)
			if (map.map[c][l].content == UNKNOWN)
			{
				*nb_l = -i;
				*nb_c = -i;
				break;
			}

		l = map.l;
		c = map.c - i;
		if (c > 0)
			if (map.map[c][l].content == UNKNOWN)
			{
				*nb_l = 0;
				*nb_c = -i;
				break;
			}

		l = map.l;
		c = map.c + i;
		if (c < map.max_c)
			if (map.map[c][l].content == UNKNOWN)
			{
				*nb_l = 0;
				*nb_c = +i;
				break;
			}

		l = map.l + i;
		c = map.c;
		if (l < map.max_l)
			if (map.map[c][l].content == UNKNOWN)
			{
				*nb_l = i;
				*nb_c = 0;
				break;
			}

		l = map.l - i;
		c = map.c;
		if (l > 0)
			if (map.map[c][l].content == UNKNOWN)
			{
				*nb_l = -i;
				*nb_c = 0;
				break;
			}

		l = map.l - i;
		c = map.c + i;
		if (c < map.max_c && l > 0)
			if (map.map[c][l].content == UNKNOWN)
			{
				*nb_l = -i;
				*nb_c = +i;
				break;
			}

		l = map.l + i;
		c = map.c - i;
		if (c > 0 && l < map.max_l)
			if (map.map[c][l].content == UNKNOWN)
			{
				*nb_l = i;
				*nb_c = -i;
				break;
			}
	}
	return (*nb_l == 0 && *nb_c == 0);
}

int test_find_target(int sim_socket, int mapper_socket)
{
	map.max_c = map.max_l = 1;
	map.l = map.c = 0;
	map.map = gen_map(map.max_l, map.max_c);

	map.map[map.c][map.l].content = ROBOT1;
	map.mapper_socket = mapper_socket;

	for (;;)
	{
		int l, c;
		if (update_map(sim_socket))
			return EXIT_FAILURE;

		update_debug_map();
		int toto=find_uncleared_zone(&l, &c);
		printf ("%d %d %d\n",toto,l,c);
		char s[30];
		scanf("%s", s);
		if (s[0] == 'q')
			break;
	}

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