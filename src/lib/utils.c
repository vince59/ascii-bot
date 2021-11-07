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
	char usage[80]; 
	sprintf(usage,"Usage : %s -s ip -p port\nex : %s -s 127.0.0.1 -p 8888\n",argv[0],argv[0]);

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

int get_mapper_param(int argc, char *argv[], int *port)
{
	int c;
	*port = 0;
	char usage[80]; 
	sprintf(usage,"Usage : %s -p port\nex : %s -p 8889\n",argv[0],argv[0]);
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

int get_srv_param(int argc, char *argv[], int *srv_port, char* mapper_srv, int *mapper_port)
{
	int c;
	char usage[200];

	*srv_port = *mapper_port = 0;
	mapper_srv = strcpy(mapper_srv, "?"); 
	sprintf(usage,"Usage : %s -p simultor_port -s mapper_server -l mapper_port \nex : %s -p 8888 -s 127.0.0.1 -l 8889\n",argv[0],argv[0]);
	while ((c = getopt(argc, argv, "s:p:l:")) != -1)
	{
		switch (c)
		{
		case 'p':
			*srv_port = atoi(optarg);
			break;
		case 'l':
			*mapper_port = atoi(optarg);
			break;
		case 's':
			mapper_srv = strcpy(mapper_srv, optarg);
			break;
		default:
			puts(usage);
			return EXIT_FAILURE;
		}
	}
	if (mapper_srv[0] == '?' || *srv_port == 0 || *mapper_port == 0)
	{
		puts(usage);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}