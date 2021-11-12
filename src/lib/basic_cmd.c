/*
** Ascii bot,
** basic_cmd.c
** File description: basic command for the robot and mapper
** 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basic_cmd.h"
#include "network.h"

//////////////////////////////////////////////////////
// Bot command

int get_id(int *id, int socket)
{
    char message[MESSAGE_LENGTH];

    if (get_message(message, socket))
        return EXIT_FAILURE;
    sscanf(message, "%d", id);
    return EXIT_SUCCESS;
}

int go_to(int direction, int *status, int socket)
{
    char message[MESSAGE_LENGTH];

    sprintf(message, "%c %d_", MOVE, direction);
    if (send_message(message, socket))
        return EXIT_FAILURE;
    if (get_message(message, socket))
        return EXIT_FAILURE;
    *status = message[0] == C_OK ? CMD_OK : CMD_KO;
    return EXIT_SUCCESS;
}

int quit(int *status, int socket)
{
    char message[MESSAGE_LENGTH];

    sprintf(message, "%c_", QUIT);
    if (send_message(message, socket))
        return EXIT_FAILURE;
    if (get_message(message, socket))
        return EXIT_FAILURE;
    *status = message[0] == C_OK ? CMD_OK : CMD_KO;
    return CMD_KO;
}

int scan(int dir, int *dist, int *info, int socket)
{
    char message[MESSAGE_LENGTH];

    sprintf(message, "%c %d_", SCAN, dir);
    if (send_message(message, socket))
        return EXIT_FAILURE;
    if (get_message(message, socket))
        return EXIT_FAILURE;
    sscanf(message, "%d %d", dist, info);
    return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////
// Mapper command

int set_cell(int col, int row, int content, int socket)
{
    char message[MESSAGE_LENGTH];

    sprintf(message, "s %d %d %d_", col, row, content);
    if (send_message(message, socket))
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////
// Simulator command

int set_id(int id, int socket)
{
    char message[MESSAGE_LENGTH];
    sprintf(message, "%d_", id);
    if (send_message(message, socket))
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int send_status(char status, int socket)
{
    char message[MESSAGE_LENGTH];
    sprintf(message, "%c_", status);
    if (send_message(message, socket))
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}