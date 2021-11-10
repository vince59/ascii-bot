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

int move(int direction, int socket)
{
    char message[MESSAGE_LENGTH];

    sprintf(message,"%c %d_",MOVE,direction);
    if (send_message(message, socket))
        return EXIT_FAILURE;
    if (get_message(message,socket))
			return EXIT_FAILURE;
    if (message[0]==OK)
        return CMD_OK;
    return CMD_KO;
}
