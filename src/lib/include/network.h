/*
** Ascii bot, 2021
** network.h
** Socket management
*/

#ifndef NETWORK_H
#define NETWORK_H

#define MESSAGE_LENGTH 100   // max string length for a message

int open_comm(char *, int, int *);
int close_comm(int);
int send_message(const char *,int);
int get_message(char *, int );
int srv_listen(int, int *);
int accept_conections(int, void *(*)(void *));

#endif