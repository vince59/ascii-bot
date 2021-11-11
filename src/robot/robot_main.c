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

int test_using_keyboard(int);
int test_detect(int);

int main(int argc, char *argv[])
{
	char server[40];
	int port, socket, id;

	if (get_bot_param(argc, argv, server, &port))
		return EXIT_FAILURE;
	if (open_comm(server, port, &socket))
		return EXIT_FAILURE;
	if (get_id(&id, socket))
		return EXIT_FAILURE;
	printf("I'm robot #%d, and i'm in the map !\n", id);

	test_detect(socket);

	puts("Bye !\n");
	close_comm(socket);
	return EXIT_SUCCESS;
}

int test_detect(int socket)
{
	int detect[24];
	if (scan(detect, socket))
		return EXIT_FAILURE;
	for (int i = 0; i < 8; i += 3)
		printf("%d %d %d\n", detect[i], detect[i + 1], detect[i + 2]);
	return EXIT_SUCCESS;
}

int test_using_keyboard(int socket)
{
	int status;
	initscr();
	/* enable pressing arrow key to generate KEY_xxx codes */
	keypad(stdscr, TRUE);
	timeout(100);
	noecho();

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

	endwin();
	if (quit(&status, socket))
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}