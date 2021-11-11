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

#include "network.h"
#include "utils.h"
#include "mapper.h"
#include "basic_cmd.h"

int main(int argc, char *argv[])
{
	char server[40];
	int port, socket, id, status;

	for (int i=-1; i<3; i++)
		if (!i)
			printf("%d Oui\n",i);
		else	
			printf("%d non\n",i);

	if (get_bot_param(argc, argv, server, &port))
		return EXIT_FAILURE;
	if (open_comm(server, port, &socket))
		return EXIT_FAILURE;
	if (get_id(&id,socket))
		return EXIT_FAILURE;

	printf("I'm robot #%d, and i'm in the map !\n",id);
	do {
		if (move(N,&status,socket))
			return EXIT_FAILURE;
	} while (status!=CMD_KO);

	if (quit(&status,socket))
			return EXIT_FAILURE;

	puts("Bye !\n");
	close_comm(socket);
	return EXIT_SUCCESS;
}
