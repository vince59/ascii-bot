/*
** Ascii bot, 2021
** network.c
** Socket management
*/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "network.h"

// Open a commucation using tcp
int open_comm(char *ip, int port, int *socket_desc)
{
	struct sockaddr_in server;
	printf("Open comm on %s:%d\n", ip, port);
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
	//printf(">%s\n",message);
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
	char buffer[2];
	int i=0;
	do
	{
		if ((recv(socket_desc, buffer, 1, 0)) < 0)
		{
			puts("Reciev failed\n");
			return EXIT_FAILURE;
		}
		message[i++]=buffer[0];
	} while (buffer[0] != '_' && buffer[0] !='\0');
	message[--i]='\0';
	//printf("<%s\n",message);
	return EXIT_SUCCESS;
}

// start server and linten to specific port

int srv_listen(int port, int *socket_desc)
{
	struct sockaddr_in server;

	//Create socket
	*socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (*socket_desc == -1)
	{
		printf("Could not create socket");
	}

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	//Bind
	if (bind(*socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("bind failed");
		return EXIT_FAILURE;
	}
	puts("bind done");

	//Listen
	listen(*socket_desc, 3);
	printf("Listen on %d\n", port);
	return EXIT_SUCCESS;
}

// accept and run thread for each new connection

int accept_conections(int socket, void *(*connection_handler)(void *))
{
	int new_socket, *new_sock, c;
	struct sockaddr_in client;

	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while ((new_socket = accept(socket, (struct sockaddr *)&client, (socklen_t *)&c)))
	{
		puts("Connection accepted");

		pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = new_socket;

		if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
		{
			perror("could not create thread");
			return EXIT_FAILURE;
		}

		puts("Handler assigned");
	}

	if (new_socket < 0)
	{
		perror("accept failed");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}