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

t_cell **map;
t_pos robots[MAX_ROBOT];
int nb_robots, max_l, max_c, mapper_socket, max_robots;

int main(int argc, char *argv[])
{
	int srv_port, mapper_port, socket;
	char mapper[40];
	char map_file[255];

	if (get_srv_param(argc, argv, &srv_port, mapper, &mapper_port, map_file))
		return EXIT_FAILURE;

	if ((map = load_map(map_file, &max_l, &max_c, &max_robots)) == NULL)
		return EXIT_FAILURE;

	if (open_comm(mapper, mapper_port, &mapper_socket))
		return EXIT_FAILURE;

	for (int col = 0; col < max_c; col++)
		for (int row = 0; row < max_l; row++)
			if (set_cell(col, row, map[col][row].obstacle, mapper_socket))
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

	int bot_id = nb_robots++;
	if (bot_id >= max_robots)
	{
		printf("Connection refused, too many robots, max = %d", max_robots);
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
		l = l1 = robots[bot_id].l;
		c = c1 = robots[bot_id].c;

		switch (message[0])
		{
		case QUIT:
			if (send_status(OK, sock))
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

			if (l < 0 || c < 0 || l >= max_l || c >= max_c || map[l][c].obstacle != FREE)
			{
				if (send_status(KO, sock))
					return NULL;
			}
			else
			{
				map[c1][l1].obstacle = FREE;
				if (set_cell(c1, l1, map[c1][l1].obstacle, mapper_socket))
					return NULL;

				map[c][l].obstacle = bot_id;
				if (set_cell(c, l, map[c][l].obstacle, mapper_socket))
					return NULL;

				robots[bot_id].l = l;
				robots[bot_id].c = c;
				if (send_status(OK, sock))
					return NULL;
			}

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