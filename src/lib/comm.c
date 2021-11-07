/*
** Ascii bot, 2021
** comm.c
** Socket management
*/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "comm.h"

// Open a commucation using tcp
int open_comm(char *ip, int port, int *socket_desc)
{
	struct sockaddr_in server;
	printf("Open comm on %s:%d\n",ip,port);
	//Create socket
	*socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (*socket_desc == -1)
	{
		puts("Could not create socket");
		return EXIT_FAILURE;
	}

	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//Connect to remote server
	if (connect(*socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("Connect error\n");
		return EXIT_FAILURE;
	}
	puts("Connected\n");
	return EXIT_SUCCESS;
}

// Close TCP com
int close_comm(int socket_desc)
{
	close(socket_desc);
	puts("Connection closed\n");
	return EXIT_SUCCESS;
}

// Write on TCP socket
int send_message(const char *message, int socket_desc)
{
	if (send(socket_desc, message, strlen(message), 0) < 0)
	{
		puts("Send failed\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

// Read on TCP socket
int get_message(char *message, int socket_desc)
{
	int n;
	if ((n = recv(socket_desc, message, sizeof(message), 0)) < 0)
	{
		puts("Reciev failed\n");
		return EXIT_FAILURE;
	}
	message[n]='\0';
	return EXIT_SUCCESS;
}
