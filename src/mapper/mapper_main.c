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

#include "network.h"
#include "utils.h"
#include "mapper.h"

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

int get_ascii(int code)
{
	switch (code)
	{
	case FREE: 
		return 32;
	case OBSTACLE:
		return 97 | A_ALTCHARSET;
	case ROBOT:
		return 88 | WA_TOP;
	}
	return '?';
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
	char cmd;
	int col, row, code;

	initscr();
	start_color();

	init_pair(FREE, COLOR_BLUE, COLOR_BLUE);
	init_pair(OBSTACLE, COLOR_RED, COLOR_RED);
	init_pair(ROBOT, COLOR_WHITE, COLOR_BLUE);

	//getmaxyx(stdscr, mrow, mcol);
	curs_set(0);
	do
	{
		if (get_message(message, sock))
			break;
		sscanf(message, "%c %d %d %d", &cmd, &col, &row, &code);
		move(row, col);
		color_set(code, NULL);
		addch(get_ascii(code));
		refresh();
	} while (message[0] != '\0');

	curs_set(1);
	endwin();
	puts("Client disconnected\n");
	free(socket);
	return EXIT_SUCCESS;
}

		