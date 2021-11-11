/*
** Ascii bot,
** basic_cmd.c
** File description: basic command for the robot
** 
*/

#include <stdio.h>
#include <stdlib.h>
#include "basic_cmd.h"
#include "network.h"

int get_id(int *id, int socket)
{
    char message[MESSAGE_LENGTH];

    if (get_message(message, socket))
        return EXIT_FAILURE;
    sscanf(message, "%d", id);
    return EXIT_SUCCESS;
}

int move(int direction, int * status, int socket)
{
    char message[MESSAGE_LENGTH];

    sprintf(message, "%c %d_", MOVE, direction);
    if (send_message(message, socket))
        return EXIT_FAILURE;
    if (get_message(message, socket))
        return EXIT_FAILURE;
    *status=message[0] == OK ? CMD_OK : CMD_KO;
    return EXIT_SUCCESS;
}

int quit(int * status, int socket)
{
    char message[MESSAGE_LENGTH];

    sprintf(message, "%c_", QUIT);
    if (send_message(message, socket))
        return EXIT_FAILURE;
    if (get_message(message, socket))
        return EXIT_FAILURE;
    *status=message[0] == OK ? CMD_OK : CMD_KO;
    return CMD_KO;
}