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
	char message[MESSAGE_LENGTH];
	char server[40];
	int port, socket, result;

	if (get_bot_param(argc, argv, server, &port))
		return EXIT_FAILURE;
	if (open_comm(server, port, &socket))
		return EXIT_FAILURE;
	
	do {
		if ((result=move(N,socket)))
			return EXIT_FAILURE;

	} while (result!=CMD_KO);

	close_comm(socket);
	return EXIT_SUCCESS;
}
