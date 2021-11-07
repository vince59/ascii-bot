/*
** Ascii bot, 2021
** main.c
** File description: mapper to display the matrix
** 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <curses.h>

#include "comm.h"
#include "utils.h"

void *connection_handler(void *);

int main(int argc, char *argv[])
{
	int port, socket;

	if (get_mapper_param(argc, argv, &port))
		return EXIT_FAILURE;
	if (srv_listen(port, &socket))
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
	//int mrow = 0, mcol = 0;
	//Get the socket descriptor
	int sock = *(int *)socket;
	char message[MESSAGE_LENGTH];
	char c, cmd;
	int col, row;

	initscr();
	//getmaxyx(stdscr, mrow, mcol);
	curs_set(0);
	do
	{
		if (get_message(message, sock))
			break;
		sscanf(message, "%c %d %d %c", &cmd, &col, &row, &c);
		move(row, col);
		addch(c);
		refresh();
	} while (message[0] != '\0');

	curs_set(1);
	endwin();
	puts("Client disconnected\n");
	free(socket);

	return EXIT_SUCCESS;
}