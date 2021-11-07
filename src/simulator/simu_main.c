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

#include "comm.h"
#include "utils.h"
#include "mapper.h"

void *connection_handler(void *);

int main(int argc, char *argv[])
{
	int srv_port, mapper_port, socket, mapper_socket;
	char mapper[40];
	char message[MESSAGE_LENGTH];
	t_cell_struct **matrix;

	if (get_srv_param(argc, argv, &srv_port, mapper, &mapper_port))
		return EXIT_FAILURE;

	if (open_comm(mapper, mapper_port, &mapper_socket))
		return EXIT_FAILURE;

	matrix = gen_map(10,20);
	init_map(matrix, 10,20);

	for (int col = 0; col < 20; col++)
	{
		for (int row = 0; row < 10; row++)
		{
			sprintf(message, "d %d %d %c_", col, row, matrix[col][row].obstacle);
			if (send_message(message, mapper_socket))
				return EXIT_FAILURE;
		}
	}

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
	char message[MESSAGE_LENGTH];

	do
	{
		if (get_message(message, sock))
			break;
		printf("%s\n", message);
		if (send_message("hello i'm the simulator_", sock))
			break;
	} while (message[0] != '\0');

	puts("Client disconnected\n");
	free(socket);
	return EXIT_SUCCESS;
}