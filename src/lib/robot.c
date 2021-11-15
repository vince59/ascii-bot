/*
** Ascii bot, 2021
** robot.c
** 
*/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>

#include "network.h"
#include "utils.h"
#include "mapper.h"
#include "basic_cmd.h"
#include "robot.h"


int find_target(int *dir, int *dist, int sim_socket)
{
    int info;
    *dir = *dist = -1;
    for (int d = 0; d < 8; d++)
    {
        if (scan(d, dist, &info, sim_socket))
            return EXIT_FAILURE;
        if (info == TARGET)
        {
            *dir = d;
            break;
        }
    }
    return EXIT_SUCCESS;
}

//////////// Test programs ////////////////

int test_detect(int sim_socket)
{
    int dist, info;

    for (int dir = 0; dir < 8; dir++)
    {
        if (scan(dir, &dist, &info, sim_socket))
            return EXIT_FAILURE;
        printf("Direction = %d, distance = %d, info = %d\n", dir, dist, info);
    }
    return EXIT_SUCCESS;
}

int test_using_keyboard(int sim_socket)
{
    int status;
    initscr();
    /* enable pressing arrow key to generate KEY_xxx codes */
    keypad(stdscr, TRUE);
    timeout(100);
    noecho();

    do
    {
        int ch = getch();
        /* if no key was waiting, ignore */
        if (ERR == ch)
            continue;
        if ('q' == ch)
            break;

        switch (ch)
        {
        case KEY_UP:
            if (go_to(N, &status, sim_socket))
                return EXIT_FAILURE;
            break;
        case KEY_DOWN:
            if (go_to(S, &status, sim_socket))
                return EXIT_FAILURE;
            break;
        case KEY_RIGHT:
            if (go_to(E, &status, sim_socket))
                return EXIT_FAILURE;
            break;
        case KEY_LEFT:
            if (go_to(O, &status, sim_socket))
                return EXIT_FAILURE;
            break;
        }

    } while (1);

    endwin();
    if (quit(&status, sim_socket))
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}