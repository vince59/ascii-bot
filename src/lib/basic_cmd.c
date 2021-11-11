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

int scan(int detect[], int socket)
{
    char message[MESSAGE_LENGTH];
    char format[255] = "";
    for (int i = 0; i < 24; i++)
        strcat(format, "%d ");

    sprintf(message, "%c_", SCAN);
    if (send_message(message, socket))
        return EXIT_FAILURE;
    if (get_message(message, socket))
        return EXIT_FAILURE;
    sscanf(message, format,
           detect[0], detect[1], detect[2], detect[3],
           detect[4], detect[5], detect[6], detect[7],
           detect[8], detect[9], detect[10], detect[11],
           detect[12], detect[13], detect[14], detect[15],
           detect[16], detect[17], detect[18], detect[19],
           detect[20], detect[21], detect[22], detect[23],
           detect[24]);
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