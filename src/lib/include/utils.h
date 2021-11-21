/*
** Ascii bot, 2021
** utils.h
** Useful functions
*/

#ifndef UTILS_H
#define UTILS_H

#define MILI_SEC 1000000L         // 1 miliseconde
#define NODE_WAIT MILI_SEC * 100L // 100 miliseconds (0.1 second)


int get_bot_param(int, char *[], char *, int *, char *, int *);
int get_srv_param(int, char *[], int *, char*, int *, char *);
int get_mapper_param(int, char *[], int *);
void short_wait();

#endif