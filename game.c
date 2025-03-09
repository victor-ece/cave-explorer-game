#include"game.h"
#include"msg.h"
#include<stdio.h>
#include<ctype.h>

#include "graph.h"
#include "libhw3.h"
#include "random.h"

/*
 * main_menu
 * 
 * Prints the main game menu and returns the user's requested action.
 */
menu_action_t main_menu() 
{
	char action;
	while (1) {
		printf(MAIN_MENU_MSG);
		scanf(" %c", &action);
		switch (toupper(action)) {
			case 'N':
				return NEW;
			case 'L':
				return LOAD;
			case 'S':
				return SAVE;
			case 'C':
				return CONTINUE;
			case 'Q':
				return QUIT;
			case 'H':
				return HELP;
			default:
				while (getchar() != '\n'); 
				printf(WRONG_SELECTION_MSG);
		}	
	}
	return QUIT; // unreachable
}

/*
 * in_game_menu
 * 
 * Prints the menu of options available while a game is in progress
 * and returns the user's requested action.
 */
game_action_t in_game_menu() 
{
	char action;
	while(1) {
		printf(GAME_MENU_MSG);
		scanf(" %c", &action);
		switch (toupper(action)) {
			case 'M': 
				return MOVE;
			case 'S': 
				return SHOOT;
			case 'E': 
				return EXIT;
			default: 
				while (getchar() != '\n');
				printf(WRONG_SELECTION_MSG);
		}
	}
}

void userInit(player_t *player)
{
	printf(NAME_REQUEST_MSG);
	scanf(" %s", player->name);

	printf(GREETING_MSG, player->name);
	player->arrows = INIT_ARROWS;
}

graph_t *gameCreate(unsigned int difficultyLevel, player_t *player, graph_t *graph)
{
	graph = create_game_graph(difficultyLevel);

	player->room = graph->vertices[0];
	player->arrows = INIT_ARROWS;

	return(graph);
}

game_status_t gamePlay(graph_t *graph, player_t *player)
{
	unsigned int nearVertices;
	int nextMove, verticeNum1, verticeNum2;
	unsigned int newVertexId;

	do {
		#ifdef DEBUG
  
		print_graph(graph);

		#endif

		nextMove = 1;

		printf(ENTER_ROOM_MSG);

		switch (player->room->contents) {
			case MINE:
				printf(TRIGGERED_MINE_MSG);
				nodeDelete(graph, player->room, player->room->id);
				player->room = graph->vertices[get_rand_range(0, graph->num_vertices - 1)];
				break;
			case PORTAL:
				printf(TRIGGERED_PORTAL_MSG);
				nodeAdd(graph, EMPTY);
				verticeNum1 = get_rand_range(0, graph->num_vertices - 1);
				do {
					verticeNum2 = get_rand_range(0, graph->num_vertices - 1);
				} while (verticeNum2 == verticeNum1);				

				nodesConnect(graph->vertices[graph->num_vertices - 1], 
				graph->vertices[verticeNum1]);
    			nodesConnect(graph->vertices[graph->num_vertices - 1],
				graph->vertices[verticeNum2]);
				
				player->room = graph->vertices[graph->num_vertices - 1];
				break;
			case DRAGON: 
				printf(ENCOUNTERED_DRAGON_MSG);
				return(GAME_OVER);
			case ARROW: 
				player->arrows++;
				printf(FOUND_ARROW_MSG, player->arrows);
				player->room->contents = EMPTY;
			case EMPTY:
				nearVertices = nearVerticesCount(player->room);
				printf(PASSAGE_INFO_MSG, nearVertices);
				nearVerticesPrint(player->room);

				if(vertexContentSearch(DRAGON, player->room)) {
					printf(DETECTED_DRAGON_MSG);
				}

				if(vertexContentSearch(PORTAL, player->room)) {
					printf(DETECTED_PORTAL_MSG);
				}

				do {
					switch(in_game_menu()) {
						case EXIT: 
							return(IN_PROGRESS);
						case MOVE:
							scanf(" %u", &newVertexId);
							if(vertexIdSearch(newVertexId ,player->room)) {
								player->room = graph->vertices[vertexIdFind(graph, newVertexId)];
								nextMove = 0; // to stop the loop
								break;
							} else {
								printf(NO_PASSAGE_MSG);
								break;
							}
						case SHOOT:
							scanf(" %u", &newVertexId);

							if(player->arrows == 0) {
								printf(NO_ARROWS_MSG);
								nextMove = 0;
								break;
							}

							if(vertexIdSearch(newVertexId ,player->room)) {
								if(graph->vertices
								[vertexIdFind(graph, newVertexId)]->contents == DRAGON) {
									printf(SHOT_DRAGON_MSG);
									return(GAME_OVER);
								} else {
									player->arrows--;
									printf(ARROW_MISSED_MSG, player->arrows);
									nextMove=0;
									//not sure if i have to stop the loop
								}

							} else {
								printf(NO_PASSAGE_MSG);
								break;
							}					
					}

				} while(nextMove);
		}
	} while (1);
}

