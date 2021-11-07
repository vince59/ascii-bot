/*
** Ascii bot, 2021
** main.c
** File description: robot main file
** 
*/

#include <ctype.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

extern  char * optarg;

int open_comm(char *, int, int *);
int close_comm(int);
int send_message(const char *,int);
int get_message(char *,int);
int get_param(int, char *[], char *, int *);

int main(int argc, char *argv[])
{
	char message[2000];
	char server[40];
	int port;
	int socket;

	if (get_param(argc, argv, server, &port)) return EXIT_FAILURE;
	if (open_comm(server, port, &socket)) return EXIT_FAILURE;
	if (send_message("h\n",socket)) return EXIT_FAILURE;
	if (get_message(message,socket)) return EXIT_FAILURE;

	puts(message);
	close_comm(socket);
	return EXIT_SUCCESS;
}

int get_param(int argc, char *argv[], char *server, int *port)
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

int close_comm(int socket_desc)
{
	close(socket_desc);
	puts("Connection closed\n");
	return EXIT_SUCCESS;
}

int send_message(const char *message, int socket_desc)
{
	if (send(socket_desc, message, strlen(message), 0) < 0)
	{
		puts("Send failed\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

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

//--------------------------------------------------------
// FUNCTION main
//--------------------------------------------------------
int test(void)
{

	int mrow = 0, mcol = 0;

	initscr();
	getmaxyx(stdscr, mrow, mcol);

	initscr();
	curs_set(0);
	for (int col = 0; col < mcol; col++)
	{
		for (int row = 0; row < mrow; row++)
		{
			move(row, col);
			addch('x');
		}
	}
	move(mrow, mcol);
	addch('0');
	refresh();
	curs_set(1);
	getch();
	endwin();
	return EXIT_SUCCESS;
}