/*
** Ascii bot, 2021
** comm.h
** Socket management
*/

#ifndef COMM_H
#define COMM_H

int open_comm(char *, int, int *);
int close_comm(int);
int send_message(const char *,int);
int get_message(char *,int);
int srv_listen(int, int *);
int accept_conections(int, void *(*)(void *));

#endif