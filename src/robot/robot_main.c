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
int test_find_target(int socket);
int find_target(int *, int *, int);

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

	test_find_target(socket);

	puts("Bye !\n");
	close_comm(socket);
	return EXIT_SUCCESS;
}

int test_find_target(int socket)
{
	int dir, dist, info, status;
	do
	{
		if (find_target(&dir, &dist, socket))
			return EXIT_FAILURE;
		if (dir > -1)
		{
			printf("Target localized at %d %d\n", dir, dist);
			do
			{
				if (go_to(dir, &status, socket))
					return EXIT_FAILURE;
			} while (status == CMD_OK);
			puts("Target catched !\n");
			return EXIT_SUCCESS;
		}
		for (int d = 0; d < 8; d++)
		{
			if (scan(d, &dist, &info, socket))
				return EXIT_FAILURE;
			if (info == 0 || (info==OBSTACLE && dist>2))
			{
				dir = d;
				break;
			}
		}
		if (go_to(dir, &status, socket))
			return EXIT_FAILURE;
	} while (1);
	return EXIT_SUCCESS;
}

int find_target(int *dir, int *dist, int socket)
{
	int info;
	*dir = *dist = -1;
	for (int d = 0; d < 8; d++)
	{
		if (scan(d, dist, &info, socket))
			return EXIT_FAILURE;
		if (info == TARGET)
		{
			*dir = d;
			break;
		}
	}
	return EXIT_SUCCESS;
}

int test_detect(int socket)
{
	int dist, info;

	for (int dir = 0; dir < 8; dir++)
	{
		if (scan(dir, &dist, &info, socket))
			return EXIT_FAILURE;
		printf("Direction = %d, distance = %d, info = %d\n", dir, dist, info);
	}
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