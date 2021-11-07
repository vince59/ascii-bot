/*
** Ascii bot, 2021
** utils.c
** Utils for robot
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <unistd.h>

#include "utils.h"

extern  char * optarg;

int get_bot_param(int argc, char *argv[], char *server, int *port)
{
	int c;
	*port = 0;
	server = strcpy(server, "?");
	char *usage = "Usage : bot -s ip -p port\nex : bot -s 127.0.0.1 -p 8888\n";
	while ((c = getopt(argc, argv, "s:p:")) != -1)
	{
		switch (c)
		{
		case 's':
			server = strcpy(server, optarg);
			break;
		case 'p':
			*port = atoi(optarg);
			break;
		default:
			puts(usage);
			return EXIT_FAILURE;
		}
	}
	if (server[0] == '?' || *port == 0)
	{
		puts(usage);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int get_srv_param(int argc, char *argv[], int *port)
{
	int c;
	*port = 0;
	char *usage = "Usage : simu -p port\nex : simu -p 8888\n";
	while ((c = getopt(argc, argv, "p:")) != -1)
	{
		switch (c)
		{
		case 'p':
			*port = atoi(optarg);
			break;
		default:
			puts(usage);
			return EXIT_FAILURE;
		}
	}
	if (*port == 0)
	{
		puts(usage);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}