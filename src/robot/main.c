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
#include <string.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>

int socket_desc;
extern  char * optarg;

int open_comm(const char *, int);
int close_comm(void);
int send_message(const char *);
int get_message(char *);

int main(int argc, char *argv[])
{

	int c;
	char *server = NULL;
	char *port = NULL;
	char *usage = "Usage : bot -s ip -p port\nex : bot -s 127.0.0.1 -p 8888\n";
	while ((c = getopt(argc, argv, "s:p:")) != -1)
	{
		switch (c)
		{
		case 's':
			server = optarg;
			break;
		case 'p':
			port = optarg;
			break;
		default:
			puts(usage);
			return EXIT_FAILURE;
		}
	}
	if (server == NULL || port == NULL)
	{
		puts(usage);
		return EXIT_FAILURE;
	}

	char message[2000];
	if (open_comm(server, atoi(port)))
		return EXIT_FAILURE;
	if (send_message("h\n"))
		return EXIT_FAILURE;
	if (get_message(message))
		return EXIT_FAILURE;

	puts(message);
	close_comm();
	return EXIT_SUCCESS;
}

int open_comm(const char *ip, int port)
{
	struct sockaddr_in server;

	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		puts("Could not create socket");
		return EXIT_FAILURE;
	}

	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//Connect to remote server
	if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("Connect error\n");
		return EXIT_FAILURE;
	}
	puts("Connected\n");
	return EXIT_SUCCESS;
}

int close_comm(void)
{
	close(socket_desc);
	puts("Connection closed\n");
	return EXIT_SUCCESS;
}

int send_message(const char *message)
{
	if (send(socket_desc, message, strlen(message), 0) < 0)
	{
		puts("Send failed\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int get_message(char *message)
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