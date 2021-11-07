/*
** Ascii bot, 2021
** main.c
** File description: robot main file
** 
*/

#include <stdio.h>
#include <string.h> 
#include <stdlib.h> 
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h>	   
#include <pthread.h> 

#include "comm.h"
#include "utils.h"

void *connection_handler(void *);

int main(int argc, char *argv[])
{
	int port, socket;

	if (get_srv_param(argc, argv, &port)) return EXIT_FAILURE;
	if (srv_listen(port, &socket)) return EXIT_FAILURE;
    if (accept_conections(socket, connection_handler)) return EXIT_FAILURE;
	
	return EXIT_SUCCESS;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
	//Get the socket descriptor
	int sock = *(int *)socket_desc;
	int read_size;
	int quit = 0;
	char *message, client_message[2000];

	//Receive a message from client
	while ((read_size = recv(sock, client_message, 2000, 0)) > 0)
	{
		client_message[read_size]='\0';
		message = "h";
		if (client_message[0] == 'h')
			write(sock, message, strlen(message));
		if (client_message[0] == 'q')
		{
			quit = 1;
			break;
		}
	}

	if (read_size == 0 || quit == 1)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if (read_size == -1)
	{
		perror("recv failed");
	}

	//Free the socket pointer
	free(socket_desc);

	return EXIT_SUCCESS;
}