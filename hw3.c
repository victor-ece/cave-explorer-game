#include"graph.h"
#include"game.h"
#include"random.h"
#include"hw3.h"
#include"msg.h"
#include<stdio.h>
#include<stdlib.h>

#include"fileio.h"

#include "libhw3.h"

int main (int argc, char *argv[]) 
{
	unsigned int level_factor, functionReturn;
	menu_action_t action;
	game_status_t gameStatus;
	player_t *player = NULL;
	graph_t *graph = NULL;
	char userInput;

	player = malloc(sizeof(player_t));

 	if (argc > 2) {
		fprintf(stderr, "Additional arguments ignored.\n");
	}
	// Get level factor (default 0) and use it to initialize the RNG
	level_factor = 0;
	if (argc == 2) {
		level_factor = atoi(argv[1]);
	}
	set_seed(level_factor);
	
	printf(WELCOME_MSG);

	// call player init function here
	userInit(player);

	do {
		action = main_menu();
		switch (action) {
			case NEW:
				if(gameStatus == IN_PROGRESS) {
					printf(CONFIRM_NEW_GAME_MSG);
					scanf(" %c", &userInput);
					if (userInput != 'y') {
						break;
					}
					graphDelete(graph, graph->num_vertices);
				}
				graph = gameCreate(level_factor, player, graph);
				gameStatus = gamePlay(graph, player);

				/* if the game ended it deletes the graph 
				(not the player or the *graph itself)*/						
				if(gameStatus == GAME_OVER) {	
					graphDelete(graph, graph->num_vertices);
					free(graph);	
				}
				break;
			case LOAD:
				if (gameStatus == IN_PROGRESS) {
					printf(CONFIRM_NEW_GAME_MSG);
					scanf(" %c", &userInput);
					if (userInput != 'y') {
						break;
					}
				}
				
				functionReturn = gameLoadCheck(player);
				if(functionReturn == 1) {
					if(gameLoad(&player, &graph, gameStatus)) {
						gameStatus = gamePlay(graph, player);
						
						/* if the game ended it deletes the graph 
						(not the player or the *graph itself)*/						
						if(gameStatus == GAME_OVER) {	
							graphDelete(graph, graph->num_vertices);
							free(graph);
						}
					} else {
						printf(CORRUPTED_ERROR_MSG);
					}
				}
				else if(functionReturn == 0){
					printf(CORRUPTED_ERROR_MSG);
				}

				break;
			case SAVE:
				if (gameStatus != IN_PROGRESS) {
					printf(NO_GAME_IN_PROGRESS_MSG);
				} else {
					if(saveCheck(player) == 0) {
						printf(FILE_EXISTS_MSG);
						scanf(" %c", &userInput);
						if (userInput != 'y') {
							break;
						}
					}

					if(gameSave(player, graph)) {
						printf(SAVE_OK_MSG);
					}
				}
				
				break;
			case CONTINUE:
				if (gameStatus != IN_PROGRESS) {
					printf(NO_GAME_IN_PROGRESS_MSG);
				} else {
					gameStatus = gamePlay(graph, player);
					if(gameStatus == GAME_OVER) {	
							graphDelete(graph, graph->num_vertices);
							free(graph);
						}
				}

				break;
			case HELP:
				printf(INSTRUCTIONS_MSG);
				break;
			case QUIT:
				if(gameStatus == IN_PROGRESS) {
					printf(CONFIRM_QUIT_MSG);
					scanf(" %c", &userInput);

					if (userInput == 'n') {
						action = CONTINUE; // in order to continue the loop not sure 
					}

					//in the loop to avoid double free if the game ended
					graphDelete(graph, graph->num_vertices);
					free(player);
					free(graph);
				} 
				else {
					free(player);
					//free(graph);
				}
				
				break;
		}
	} while (action != QUIT);

	return 0;
}

/*
void newGame()
{
	graph_t *graph;

	graph = create_game_graph(1);

	print_graph(graph);

	nodeAdd(graph);

	print_graph(graph);

	nodesConnect(graph->vertices[graph->num_vertices - 1], graph->vertices[graph->num_vertices - 2]);
	//nodesConnect(graph->vertices[3], graph->vertices[5]);
	//nodesConnect(graph->vertices[4], graph->vertices[6]);

	print_graph(graph);

	nodesDisconnect(graph->vertices[graph->num_vertices - 1], graph->vertices[graph->num_vertices - 2], graph);

	//nodesDisconnect(graph->vertices[3], graph->vertices[5]);
	//nodesDisconnect(graph->vertices[4], graph->vertices[6]);

	print_graph(graph);

	nodeDelete(graph, graph->vertices[graph->num_vertices - 3], graph->vertices[graph->num_vertices -3]->id);

	//nodeDelete(graph, graph->vertices[0]);

	print_graph(graph);

	graphDelete(graph);

	//print_graph(graph);
}*/
