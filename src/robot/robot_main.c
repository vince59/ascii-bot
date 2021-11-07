/*
** Ascii bot, 2021
** main.c
** File description: robot main file
** 
*/

#include <ctype.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "comm.h"
#include "utils.h"

int main(int argc, char *argv[])
{
	char message[2000];
	char server[40];
	int port;
	int socket;

	if (get_bot_param(argc, argv, server, &port)) return EXIT_FAILURE;
	if (open_comm(server, port, &socket)) return EXIT_FAILURE;
	if (send_message("h\n",socket)) return EXIT_FAILURE;
	if (get_message(message,socket)) return EXIT_FAILURE;

	puts(message);
	close_comm(socket);
	return EXIT_SUCCESS;
}

//--------------------------------------------------------
// FUNCTION main
//--------------------------------------------------------
int test(void)
{

	int mrow = 0, mcol = 0;

	initscr();
	getmaxyx(stdscr, mrow, mcol);

	initscr();
	curs_set(0);
	for (int col = 0; col < mcol; col++)
	{
		for (int row = 0; row < mrow; row++)
		{
			move(row, col);
			addch('x');
		}
	}
	move(mrow, mcol);
	addch('0');
	refresh();
	curs_set(1);
	getch();
	endwin();
	return EXIT_SUCCESS;
}