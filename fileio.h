#include "game.h"

#ifndef _HW3_FILEIO
#define _HW3_FILEIO

#define MAGIC_NUMBER "Dr460N5D3N"

/*
 * Checks if there is an error from the write function. If there was an error and prints the 
 * error and returns.
 * Parameters: The return int from the write and the file descriptor (to close the file)
 * Return: Nothing
*/
void writeCheck(int returnAmount, int fileDescriptor);

/*
 * Saves in a file the info of a game (graph and players position)
 * Parameters:
 * Returns: 1 if everything went okay
*/
int gameSave(player_t *player, graph_t *graph);

/*
 * Creates the name of the file we want to open based on the players name
 * Parameters: The string where we will store the filename and the players name
 * Returns: Nothing
*/
void filenameCreate(char *filename, char *playerName);

/*
 * Reads from a file the info of a game and initilises a graph and the players position
 * Parameters:
 * Returns: 1 if everything is OK otherwise 0
*/
int gameLoad(player_t **oldPlayer, graph_t **oldGraph, game_status_t gameStatus);

/*
 * Checks if the file exists and has the right structure
 * Parameters:
 * Returns: 1 if everything is OK otherwise 0
*/
int gameLoadCheck(player_t *player);

/*
 * Checks if there already exists a file with the same name
 * Parameters: The filename
 * Returns: 1 if the file doesn't exist 0 if it exists and 2 if there is another type of error
*/
int saveCheck(player_t *player);


#endif
