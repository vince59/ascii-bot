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

int main(int argc, char *argv[])
{
	char server[40];
	int port, socket, id, status;

	for (int i = -1; i < 3; i++)
		if (!i)
			printf("%d Oui\n", i);
		else
			printf("%d non\n", i);

	if (get_bot_param(argc, argv, server, &port))
		return EXIT_FAILURE;
	if (open_comm(server, port, &socket))
		return EXIT_FAILURE;
	if (get_id(&id, socket))
		return EXIT_FAILURE;
	initscr();
	/* enable pressing arrow key to generate KEY_xxx codes */
	keypad(stdscr, TRUE);
	timeout(100);
	noecho();

	printf("I'm robot #%d, and i'm in the map !\n", id);
	do
	{
		int ch = getch();
		/* if no key was waiting, ignore */
		if (ERR == ch)
			continue;
		if ('q' == ch)
			break;

		switch (ch)
		{ 
		case KEY_UP:
			if (go_to(N, &status, socket))
				return EXIT_FAILURE;
			break;
		case KEY_DOWN:
			if (go_to(S, &status, socket))
				return EXIT_FAILURE;
			break;
		case KEY_RIGHT:
			if (go_to(E, &status, socket))
				return EXIT_FAILURE;
			break;
		case KEY_LEFT:
			if (go_to(O, &status, socket))
				return EXIT_FAILURE;
			break;
		}

	} while (1);

	if (quit(&status, socket))
		return EXIT_FAILURE;
	endwin();
	puts("Bye !\n");
	close_comm(socket);
	return EXIT_SUCCESS;
}
