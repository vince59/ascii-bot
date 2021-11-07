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

#include "comm.h"
#include "utils.h"

int main(int argc, char *argv[])
{
	char message[MESSAGE_LENGTH];
	char server[40];
	int port;
	int socket;

	if (get_bot_param(argc, argv, server, &port))
		return EXIT_FAILURE;
	if (open_comm(server, port, &socket))
		return EXIT_FAILURE;
	for (int i = 0; i < 10; i++)
	{
		if (send_message("hello i'm the bot_", socket))
			return EXIT_FAILURE;
		if (get_message(message,socket))
			return EXIT_FAILURE;
		printf("%s\n",message);
	}
	close_comm(socket);
	return EXIT_SUCCESS;
}
