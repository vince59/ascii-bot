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

t_cell **map;
t_pos robots[MAX_ROBOT];
int nb_robots, max_l, max_c, mapper_socket;

int main(int argc, char *argv[])
{
	int srv_port, mapper_port, socket;
	char mapper[40];
	char message[MESSAGE_LENGTH];

	nb_robots = 0;
	max_l = 10;
	max_c = 20;
	if (get_srv_param(argc, argv, &srv_port, mapper, &mapper_port))
		return EXIT_FAILURE;

	if (open_comm(mapper, mapper_port, &mapper_socket))
		return EXIT_FAILURE;

	map = gen_map(max_l, max_c);
	init_map(map, max_l, max_c);

	for (int col = 0; col < max_c; col++)
	{
		for (int row = 0; row < max_l; row++)
		{
			sprintf(message, "s %d %d %d_", col, row, map[col][row].obstacle);
			if (send_message(message, mapper_socket))
				return EXIT_FAILURE;
		}
	}
robots[0].l=7;
robots[0].c=4;
robots[1].l=5;
robots[1].c=3;


	    map[4][7].obstacle = ROBOT1;
    map[3][5].obstacle = ROBOT2;

sprintf(message, "s %d %d %d_", robots[0].c, robots[0].l, ROBOT1);
			if (send_message(message, mapper_socket))
				return EXIT_FAILURE;
sprintf(message, "s %d %d %d_", robots[1].c, robots[1].l, ROBOT2);
			if (send_message(message, mapper_socket))
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
	int end=0;
	char message[MESSAGE_LENGTH], cmd;

	int bot_id = nb_robots++;

	sprintf(message, "%d_", bot_id);
	if (send_message(message, sock))
		return NULL;

	printf("Robot #%d connected\n",bot_id);

	do
	{
		if (get_message(message, sock))
			break;
		int l,c,l1,c1;
		int dir;
		l = l1 = robots[bot_id].l;
		c = c1 = robots[bot_id].c;
	
		switch (message[0])
		{
		case QUIT :
			sprintf(message, "%c_", KO);
				send_message(message, sock);
			end=1;
			break;
		case MOVE:
			sscanf(message, "%c %d", &cmd, &dir);
			switch (dir)
			{
			case N:
				l--;
				break;
			case NE:
				l--;
				c++;
				break;
			case E:
				c++;
				break;
			case SE:
				l++;
				c++;
				break;
			case S:
				l++;
				break;
			case SO:
				l++;
				c--;
				break;
			case O:
				c--;
				break;
			case NO:
				l--;
				break;
			default:
				printf("Protocol error robot #%d unknown command in message :%s\n", bot_id, message);
			}
			if (l < 0 || c < 0 || l >= max_l || c >= max_c || map[l][c].obstacle != FREE)
			{
				sprintf(message, "%c_", KO);
				send_message(message, sock);
			}
			else
			{
			map[c1][l1].obstacle = FREE;	
			sprintf(message, "s %d %d %d_", c1, l1, map[c1][l1].obstacle);
			if (send_message(message, mapper_socket))
			{
				return NULL;
			}

			map[c][l].obstacle = bot_id;
			sprintf(message, "s %d %d %d_", c, l, map[c][l].obstacle);
			if (send_message(message, mapper_socket))
			{
				return NULL;
			}

			robots[bot_id].l=l;
			robots[bot_id].c=c;
			sprintf(message, "%c_", OK);
			if (send_message(message, sock))
			{
				return NULL;
			}
			}
			

			break;
		default:
			printf("Protocol error robot #%d\n", bot_id);
		}
	} while (message[0] != '\0' && end!=1);

	puts("Client disconnected\n");
	free(socket);
	return EXIT_SUCCESS;
}