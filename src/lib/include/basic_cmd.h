/*
** Ascii bot,
** basic_cmd.h
** File description: basic command for the robot
** 
*/

#define MOVE 'm'
#define QUIT 'q'
#define C_OK 'o'
#define C_KO 'k'

#define CMD_OK 1
#define CMD_KO 0

int go_to(int, int *, int);
int quit(int *, int);
int get_id(int *, int);
int set_cell(int, int, int, int);
int set_id(int, int);
int send_status(char, int);
