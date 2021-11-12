/*
** Ascii bot, 2021
** main.c
** File description: robot main file
** 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "network.h"
#include "utils.h"
#include "mapper.h"
#include "basic_cmd.h"
#include "simulator.h"

void *connection_handler(void *);
int get_next_cell(int, int *, int *);
int load_map(char *file_name);

t_map map;

int main(int argc, char *argv[])
{
	int srv_port, mapper_port, socket;
	char mapper[40];
	char map_file[255];

	if (get_srv_param(argc, argv, &srv_port, mapper, &mapper_port, map_file))
		return EXIT_FAILURE;

	if (load_map(map_file))
		return EXIT_FAILURE;

	if (open_comm(mapper, mapper_port, &map.mapper_socket))
		return EXIT_FAILURE;

	for (int col = 0; col < map.max_c; col++)
		for (int row = 0; row < map.max_l; row++)
			if (set_cell(col, row, map.map[col][row].obstacle, map.mapper_socket))
				return EXIT_FAILURE;

	if (srv_listen(srv_port, &socket))
		return EXIT_FAILURE;
	if (accept_conections(socket, connection_handler))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket)
{
	//Get the socket descriptor
	int sock = *(int *)socket;
	int end = 0;
	char message[MESSAGE_LENGTH], cmd;

	int bot_id = map.nb_robots++;
	if (bot_id >= map.max_robots)
	{
		printf("Connection refused, too many robots, max = %d", map.max_robots);
		return NULL;
	}

	if (set_id(bot_id, sock))
		return NULL;

	printf("Robot #%d connected\n", bot_id);

	do
	{
		if (get_message(message, sock))
			return NULL;
		int l, c, l1, c1;
		int dir;
		l = l1 = map.robots[bot_id].l;
		c = c1 = map.robots[bot_id].c;

		switch (message[0])
		{
		case QUIT:
			if (send_status(C_OK, sock))
				return NULL;
			end = 1;
			break;
		case MOVE:
			sscanf(message, "%c %d", &cmd, &dir);
			if (get_next_cell(dir, &l, &c))
			{
				printf("Protocol error robot #%d unknown command in message :%s\n", bot_id, message);
				return NULL;
			}

			if (l < 0 || c < 0 || l >= map.max_l || c >= map.max_c || map.map[c][l].obstacle != FREE)
			{
				if (send_status(C_KO, sock))
					return NULL;
			}
			else
			{
				map.map[c1][l1].obstacle = FREE;
				if (set_cell(c1, l1, map.map[c1][l1].obstacle, map.mapper_socket))
					return NULL;

				map.map[c][l].obstacle = bot_id;
				if (set_cell(c, l, map.map[c][l].obstacle, map.mapper_socket))
					return NULL;

				map.robots[bot_id].l = l;
				map.robots[bot_id].c = c;
				if (send_status(C_OK, sock))
					return NULL;
			}

			break;
		case SCAN:
			sscanf(message, "%c %d", &cmd, &dir);
			int out = 0, detected = 0, dist;
			for (dist = 1; dist<=MAX_SCAN_DIST; dist++)
			{
				get_next_cell(dir, &l, &c);
				if ((out = (l < 0 || c < 0 || l >= map.max_l || c >= map.max_c)))
					break;
				if ( (detected=(map.map[c][l].obstacle != FREE)))
					break;
			}
			sprintf(message, "%d %d_", dist, detected ? map.map[c][l].obstacle : detected);
			if (send_message(message, sock))
				return NULL;
			break;
		default:
			printf("Protocol error robot #%d unknown command in message :%s\n", bot_id, message);
		}
	} while (message[0] != '\0' && end != 1);

	puts("Client disconnected\n");
	free(socket);
	return NULL;
}

int get_next_cell(int dir, int *l, int *c)
{
	switch (dir)
	{
	case N:
		(*l)--;
		break;
	case NE:
		(*l)--;
		(*c)++;
		break;
	case E:
		(*c)++;
		break;
	case SE:
		(*l)++;
		(*c)++;
		break;
	case S:
		(*l)++;
		break;
	case SO:
		(*l)++;
		(*c)--;
		break;
	case O:
		(*c)--;
		break;
	case NO:
		(*l)--;
		break;
	default:
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int load_map(char *file_name)
{
	int c;
	FILE *file;
	file = fopen(file_name, "r");
	map.max_l = 0;
	map.max_c = 0;
	map.max_robots = 0;
	if (file)
	{
		while ((c = getc(file)) != EOF)
		{
			if (c == '\n')
			{
				map.max_l++;
			}
			else
			{
				if (map.max_l == 0)
					map.max_c++;
				if (c != ' ' && c != 'X')
					map.max_robots++;
			}
		}
		printf("Max rows = %d, max cols = %d, max bots = %d\n", map.max_l, map.max_c, map.max_robots);
		map.map = gen_map(map.max_l, map.max_c);
		init_map(map.map, map.max_l, map.max_c);

		rewind(file);
		int col = 0, row = 0;

		while ((c = getc(file)) != EOF)
		{
			switch (c)
			{
			case '\n':
				col = 0;
				row++;
				break;
			case 'X':
				map.map[col++][row].obstacle = OBSTACLE;
				break;
			case '0':
				map.robots[0].l = row;
				map.robots[0].c = col;
				map.map[col++][row].obstacle = 0;
				break;
			case '1':
				map.robots[1].l = row;
				map.robots[1].c = col;
				map.map[col++][row].obstacle = 1;
				break;
			case 'T':
				map.map[col++][row].obstacle = TARGET;
				break;
			default:
				col++;
			}
		}

		fclose(file);
	}
	else
	{
		puts("Error when loding map, check map file path\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}