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
#include <math.h>

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

// Send all map to the mapper
// map is internal matrix for the robot memory

int update_debug_map()
{
	for (int l = 0; l < map.max_l; l++)
		for (int c = 0; c < map.max_c; c++)
			if (set_cell(c, l, map.map[c][l].count > 0 && map.map[c][l].content != ROBOT1 ? TRACE : map.map[c][l].content, map.mapper_socket))
				return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

// enlarge matrix from the robot postion to dist
// dist is a number of cells
// North

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

// East

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

// South
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

// West
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

// Scan area and enlarge matrix if necesary
// sim_socket : simulator socket
int update_map(int sim_socket)
{
	int dist, info;
	for (int d = 0; d < 8; d++)
	{
		if (scan(d, &dist, &info, sim_socket))
			return EXIT_FAILURE;
		//printf(">> %d %d %d\n",d, dist, info);
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

// get the shortest path to go from x0, and y0 to x1 and y1
// dots : array with all the plot for the path
// dots[0][0] -> column (x) of the first plot
// dots[0][1] -> row (y) of the first plot
// dots[1][0] -> column (x) of the second plot
// dots[1][1] -> row (y) of the second plot
// dots[n][0] -> column (x) of the n plot
// dots[n][1] -> row (y) of the n plot
// returns the number of plot
// dots have to be allocated before the call -> int dots[10][2] for 10 plots max

int get_path(int x0, int y0, int x1, int y1, int (*dots)[2])
{
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, i = 0, e2;

	for (;;)
	{
		if (dots != NULL && i > 0)
		{
			dots[i - 1][0] = x0;
			dots[i - 1][1] = y0;
		}
		i++;
		if (x0 == x1 && y0 == y1)
			break;
		e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			y0 += sy;
		}
	}
	return i - 1;
}

// find the closest unknown point
// gives the row number and the colum number of this point
// returns 0 if all area has been already discovered, 1 otherwhise

int find_uncleared_zone(int *min_l, int *min_c)
{
	int dis = 9999;
	int path[map.max_l * map.max_c][2];
	for (int l = 0; l < map.max_l; l++)
		for (int c = 0; c < map.max_c; c++)
		{
			if (map.map[c][l].content == UNKNOWN)
			{
				int d = get_path(map.c, map.l, c, l, path);
				int k = 0;
				for (int j = 0; j < d; j++)
				{
					if (map.map[path[j][0]][path[j][1]].content == OBSTACLE)
					{
						k++;
						break;
					}
				}

				//if (k == 1)
				//	printf("%d\n", k);
				if (k == 0 && d < dis)
				{
					dis = d;
					*min_l = l;
					*min_c = c;
				}
			}
		}
	return (dis == 9999);
}

// check if coordonate in not outside the matrix

int out_of_bound_x(int x)
{
	return ((x < 0) || (x >= map.max_c));
}

int out_of_bound_y(int y)
{
	return ((y < 0) || (y >= map.max_l));
}

int out_of_bound(int x, int y)
{
	return (out_of_bound_x(x) || out_of_bound_y(y));
}

// Find the closest unknown cell from the bot cell
// AAAAAAA
// B     D
// B  X  D
// B     D
// CCCCCCC

int find_unknown_cell(int *x, int *y, int level)
{
	int x0 = map.c - level;
	int y0 = map.l + level;
	int x1 = map.c + level;
	int y1 = map.l - level;
	int length = level * 2 + 1;

	int stop = 0;
	printf("---- %d ----- %d \n",level, length);
	// A
	if (!out_of_bound_y(y1))
	{
		for (int i = 0; i < length; i++)
		{
			int ix0 = x0 + i;
			printf("AA x = %d, y = %d\n", ix0,y1);
			if (!out_of_bound_x(ix0) && map.map[ix0][y1].content == UNKNOWN)
			{
				*x = ix0;
				*y = y1;
				printf("Target x=%d,y=%d\n",*x,*y);
				return TRUE;
			}
		}
	}
	else
	{
		stop++;
		printf("y1 %d\n", y1);
	}

	// B
	if (!out_of_bound_x(x0))
	{
		for (int i = 0; i < length; i++)
		{
			int iy0 = y1 + i;
			printf("BB x = %d, y = %d\n", x0,iy0);
			if (!out_of_bound_y(iy0) && map.map[x0][iy0].content == UNKNOWN)
			{
				*x = x0;
				*y = iy0;
				printf("Target x=%d,y=%d\n",*x,*y);
				return TRUE;
			}
		}
	}
	else
			{
		stop++;
		printf("x0 %d\n", x0);
	}

	// C
	if (!out_of_bound_y(y0))
	{
		for (int i = 0; i < length; i++)
		{
			int ix0 = x0 + i;
			printf("CC x = %d, y = %d\n", ix0,y0);
			if (!out_of_bound_x(ix0) && map.map[ix0][y0].content == UNKNOWN)
			{
				*x = ix0;
				*y = y0;
				printf("Target x=%d,y=%d\n",*x,*y);
				return TRUE;
			}
		}
	}
	else
			{
		stop++;
		printf("y0 %d\n", y0);
	}

	// D
	if (!out_of_bound_x(x1))
	{
		for (int i = 0; i < length; i++)
		{
			int iy0 = y1 + i;
			printf("DD x = %d, y = %d\n", x1,iy0);
			if (!out_of_bound_y(iy0) && map.map[x1][iy0].content == UNKNOWN)
			{
				*x = x1;
				*y = iy0;
				printf("Target x=%d,y=%d\n",*x,*y);
				return TRUE;
			}
		}
	}
	else
		{
		stop++;
		printf("x1 %d\n", x1);
	}

	if (stop < 4)
		return find_unknown_cell(x, y, level + 1);
	return FALSE;
}

int push_cell(int i, int x, int y, int (*cells)[2])
{
	if (!out_of_bound(x, y) && map.map[x][y].content == FREE && map.map[x][y].count == 0)
	{
		cells[i][0] = x;
		cells[i][1] = y;
		i++;
	}
	return i;
}

int find_all_free_adjacent_cells(int x, int y, int (*cells)[2])
{
	int i = 0;
	i = push_cell(i, x - 1, y - 1, cells);
	i = push_cell(i, x, y - 1, cells);
	i = push_cell(i, x + 1, y - 1, cells);

	i = push_cell(i, x - 1, y, cells);
	i = push_cell(i, x + 1, y, cells);

	i = push_cell(i, x - 1, y + 1, cells);
	i = push_cell(i, x, y + 1, cells);
	i = push_cell(i, x + 1, y + 1, cells);
	return i;
}

// find the direction to go to join a specific point
// r_l : row of the start point
// r_c : column of the start point
// l : row of the destination point
// c : column of the destination point
// return the direction N S E O ...

int get_dir_to_cell(int r_l, int r_c, int l, int c)
{
	int d_l = r_l - l;
	int d_c = r_c - c;

	if (d_l == 0)
	{
		if (d_c > 0)
			return O;
		return E;
	}
	if (d_c == 0)
	{
		if (d_l > 0)
			return N;
		return S;
	}
	if (d_l > 0 && d_c > 0)
		return NO;
	if (d_l > 0 && d_c < 0)
		return NE;
	if (d_l < 0 && d_c > 0)
		return SO;
	return SE;
}

// get digonal length (pythagore therorem)
int get_digonal_length()
{
	return sqrt((map.max_c ^ 2) + (map.max_l ^ 2));
}

void clear_count()
{
	for (int l = 0; l < map.max_l; l++)
		for (int c = 0; c < map.max_c; c++)
			map.map[c][l].count = 0;
}

// test program to find a target

int test_find_target(int sim_socket, int mapper_socket)
{
	map.max_c = map.max_l = 1;
	map.l = map.c = 0;
	map.map = gen_map(map.max_l, map.max_c);

	map.map[map.c][map.l].content = ROBOT1;
	map.map[map.c][map.l].count++;
	map.mapper_socket = mapper_socket;

	for (;;)
	{
		int l, c;
		if (update_map(sim_socket)) // scan area and update internal matrix
			return EXIT_FAILURE;

		update_debug_map(); // display updated map with the mapper
		puts("New target\n");
		clear_count();
		if (!find_unknown_cell(&c, &l, 1)) // find the closest unknown cell from the bot
		{
			puts("All area discovered\n");
			break; // all the area has been discovered
		}

		for (;;)
		{
			int cells[8][2];
			int k = find_all_free_adjacent_cells(map.c, map.l, cells);
			if (k == 0)
				break; // bot seems to be in jail ???
			int dist = 9999, min = 0;
			for (int m = 0; m < k; m++) // iterate on each free adjacent cells
			{
				int d = get_path(cells[m][0], cells[m][1], c, l, NULL);
				if (d < dist)
				{
					dist = d;
					min = m;
				}
			}

			if (dist == 9999)
				break; // is it possible ???

			// get the direction to go the closest free cell form the bot to the closest unknown cells
			int dir = get_dir_to_cell(map.l, map.c, cells[min][1], cells[min][0]);

			// go to the closest free cell form the bot to the closest unknown cells
			int status;
			if (go_to(dir, &status, sim_socket))
				return EXIT_FAILURE;
			if (status != CMD_OK)
			{
				puts("bing!\n");
				return EXIT_FAILURE;
			}
			/*if (s[0] == 'q')
				break;*/
			map.map[map.c][map.l].content = FREE;
			map.l = cells[min][1];
			map.c = cells[min][0];
			map.map[map.c][map.l].content = ROBOT1;
			map.map[map.c][map.l].count++;

			if (update_map(sim_socket))
				return EXIT_FAILURE;
			update_debug_map();
			set_cell(c, l, TARGET, map.mapper_socket);
			char s[30];
			scanf("%s", s);
			if (map.l == l && map.c == c)
			{
				puts("target found !!\n");
				break;
			}
			if (map.map[c][l].content == OBSTACLE)
			{
				puts("target in wall !!\n");
				break;
			}
		}
	}

	puts("fini\n");
	char s[30];
	scanf("%s", s);
	/*if (s[0] == 'q')
				break;*/
	return EXIT_SUCCESS;
}

int move_to(int direction, int *status, int sim_socket)
{

	if (go_to(direction, status, sim_socket))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}