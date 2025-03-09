#ifndef _HW3_GAME
#define _HW3_GAME

#include"graph.h"

#ifndef INIT_ARROWS
#define INIT_ARROWS 2
#endif

#define NAME_SIZE 32

typedef enum {
	GAME_OVER, IN_PROGRESS
} game_status_t;

typedef enum {
    QUIT, NEW, LOAD, SAVE, CONTINUE, HELP
} menu_action_t;

typedef enum {
    EXIT, MOVE, SHOOT
} game_action_t;
                                                                                
typedef struct {
    char name[NAME_SIZE];
	unsigned int arrows;
	vertex_t *room;
} player_t;


/*
 * main_menu
 *
 * Prints the main game menu and returns the user's requested action.
 */
menu_action_t main_menu();

/*
 * in_game_menu
 * 
 * Prints the menu of options available while a game is in progress
 * and returns the user's requested action.
 */
game_action_t in_game_menu();

void userInit(player_t *player);

graph_t *gameCreate(unsigned int difficultyLevel, player_t *player, graph_t *graph);

game_status_t gamePlay(graph_t *graph, player_t *player);
	
#endif
