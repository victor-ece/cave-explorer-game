#include"fileio.h"

#include "errno.h"
#include "stdio.h"
#include "game.h"
#include "string.h"
#include "graph.h"
#include "msg.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "stdlib.h"

#include "libhw3.h"

/*
 * Checks if there is an error from the write function. If there was an error and prints the 
 * error and returns.
 * Parameters: The return int from the read and the file descriptor (to close the file)
 * Return: 0 if it has reached the EOF or 1 if it hasn't or -1 if the read failed
*/
int readCheck(int returnAmount, int fileDescriptor) 
{
    if(returnAmount == - 1) {
        close(fileDescriptor);
        perror(READ_ERROR_MSG);
        return(-1);
    }

    if(returnAmount == 0) {
        return(0);
    } else {
        return(1);
    }
}

/*
 * Checks if there is an error from the read function. If there was an error and prints the 
 * error and returns.
 * Parameters: The return int from the write and the file descriptor (to close the file)
 * Return: Nothing
*/
void writeCheck(int returnAmount, int fileDescriptor) 
{
    if(returnAmount == - 1) {
        close(fileDescriptor);
        perror(WRITE_ERROR_MSG);
        return;
    }
}

/*
 * Creates the name of the file we want to open based on the players name
 * Parameters: The string where we will store the filename and the players name
 * Returns: Nothing
*/
void filenameCreate(char *filename, char *playerName)
{
    int nameLength, i;

    nameLength = strlen(playerName);
    for(i=0; i < nameLength; i++) {
        filename[i] = playerName[i];
    }
    filename[nameLength] = '.';
    filename[nameLength + 1] = 's';
    filename[nameLength + 2] = 'a'; 
    filename[nameLength + 3] = 'v';
    filename[nameLength + 4] = '\0';
}

/*
 * Saves in a file the info of a game (graph and players position)
 * Parameters:
 * Returns: 1 if everything went right or 0 if it didn't
*/
int gameSave(player_t *player, graph_t *graph)
{
    int i, fileDescriptor, writeAmount;
    unsigned int nearVertices;
    char filename[NAME_SIZE + 4]; // ".sav" = 4 chars
    edge_t *currEdge;

    filenameCreate(filename, player->name);

    fileDescriptor = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);

    if (fileDescriptor == -1) {
        close(fileDescriptor);
        perror(OPEN_ERROR_MSG);
        return(0);
    }

    writeAmount = write(fileDescriptor, MAGIC_NUMBER, 10);
    writeCheck(writeAmount, fileDescriptor);

    writeAmount = write(fileDescriptor, &player->arrows, 4);
    writeCheck(writeAmount, fileDescriptor);

    writeAmount = write(fileDescriptor, &player->room->id, 4);
    writeCheck(writeAmount, fileDescriptor);
    
    writeAmount = write(fileDescriptor, &graph->max_id, 4);
    writeCheck(writeAmount, fileDescriptor);

    writeAmount = write(fileDescriptor, &graph->num_vertices, 4);
    writeCheck(writeAmount, fileDescriptor);
    
    writeAmount = write(fileDescriptor, MAGIC_NUMBER, 10);
    writeCheck(writeAmount, fileDescriptor);

    for (i=0; i < graph->num_vertices; i++) {
        writeAmount = write(fileDescriptor, &graph->vertices[i]->id, 4);
        writeCheck(writeAmount, fileDescriptor);

        writeAmount = write(fileDescriptor, &graph->vertices[i]->contents, sizeof(contents_t));
        writeCheck(writeAmount, fileDescriptor);
    }

    for (i=0; i < graph->num_vertices; i++) {
        nearVertices = nearVerticesCount(graph->vertices[i]);
        writeAmount = write(fileDescriptor, &nearVertices, 4);
        writeCheck(writeAmount, fileDescriptor);

        for (currEdge = graph->vertices[i]->edges; currEdge->next != NULL; currEdge = currEdge->next) {
            writeAmount = write(fileDescriptor, &currEdge->to->id, 4);
            writeCheck(writeAmount, fileDescriptor);
        }
        writeAmount = write(fileDescriptor, &currEdge->to->id, 4);
        writeCheck(writeAmount, fileDescriptor);
    }

    writeAmount = write(fileDescriptor, MAGIC_NUMBER, 10);
    writeCheck(writeAmount, fileDescriptor);

    close(fileDescriptor);
    return(1);
}

/*
 * Reads from a file the info of a game and initilises a graph and the players position
 * Parameters:
 * Returns: 1 if everything is OK otherwise 0
*/
int gameLoad(player_t **oldPlayer, graph_t **oldGraph, game_status_t gameStatus)
{
    int i, j, k, lowestIdPos, readReturn, checkReturn, fileDescriptor;
    unsigned int playerRoomId, nearVertices, *idsTable; 
    char filename[NAME_SIZE + 4]; // ".sav" = 4 chars
    player_t *player;
    graph_t *graph;

    player = malloc(sizeof(player_t));
    graph = malloc(sizeof(graph_t));

    filenameCreate(filename, (*oldPlayer)->name);

    fileDescriptor = open(filename, O_RDONLY , 0644);

    if (fileDescriptor == -1) {
        close(fileDescriptor);
        perror(OPEN_ERROR_MSG);
        return(0);
    }

   lseek(fileDescriptor, 10, SEEK_CUR);

    readReturn = read(fileDescriptor, &player->arrows, 4);
    checkReturn = readCheck(readReturn, fileDescriptor);
    if(!checkReturn) {
        free(graph);
        free(player);
        return(0);
    } else if (checkReturn == -1) {
        free(graph);
        free(player);
        return(0);
    }
    //printf("player arrows %u\n", player->arrows);


    readReturn = read(fileDescriptor, &playerRoomId, 4);
    checkReturn = readCheck(readReturn, fileDescriptor);
    if(!checkReturn) {
        return(0);
    } else if (checkReturn == -1) {
        free(graph);
        free(player);
        return(0);
    }
    //printf("player room id %u\n", playerRoomId);


    readReturn = read(fileDescriptor, &graph->max_id, 4);
    checkReturn = readCheck(readReturn, fileDescriptor);
    if(!checkReturn) {
        free(graph);
        free(player);
        return(0);
    } else if (checkReturn == -1) {
        free(graph);
        free(player);
        return(0);
    }

    //printf("graph max id %u\n", graph->max_id);


    readReturn = read(fileDescriptor, &graph->num_vertices, 4);
    checkReturn = readCheck(readReturn, fileDescriptor);
    if(!checkReturn) {
        free(graph);
        free(player);
        return(0);
    } else if (checkReturn == -1) {
        free(graph);
        free(player);
        return(0);
    }

    //printf("graph num vertices %u\n", graph->num_vertices);
    //printf("afterhere\n");

    lseek(fileDescriptor, 10, SEEK_CUR);
    
    graph->vertices = malloc(sizeof(vertex_t*)*graph->num_vertices);

    for (i=0; i < graph->num_vertices; i++) {
        graph->vertices[i] = malloc(sizeof(vertex_t));

        readReturn = read(fileDescriptor, &graph->vertices[i]->id, 4);
        checkReturn = readCheck(readReturn, fileDescriptor);
        if(!checkReturn) {
            for(k=0; k <= i; k++) {
                free(graph->vertices[k]);
            }
            free(graph);
            free(player);
            return(0);
        } else if (checkReturn == -1) {
            for(k=0; k <= i; k++) {
                free(graph->vertices[k]);
            }
            free(graph->vertices);
            free(graph);
            free(player);
            return(0);
        }

        readReturn = read(fileDescriptor, &graph->vertices[i]->contents, sizeof(contents_t));
        checkReturn = readCheck(readReturn, fileDescriptor);
        if(!checkReturn) {
            for(k=0; k <= i; k++) {
                free(graph->vertices[k]);
            }
            free(graph->vertices);
            free(graph);
            free(player);            
            return(0);
        } else if (checkReturn == -1) {
            for(k=0; k <= i; k++) {
                free(graph->vertices[k]);
            }
            free(graph->vertices);
            free(graph);
            free(player);
            return(0);
        }
    }

        //printf("afterhere2\n");

    for (i=0; i < graph->num_vertices; i++) {
        //nearVertices = nearVerticesCount(graph->vertices[i]);
        readReturn = read(fileDescriptor, &nearVertices, 4);
        checkReturn = readCheck(readReturn, fileDescriptor);
        if(!checkReturn) {
            free(graph->vertices);
            free(graph);
            free(player);
            return(0);
        } else if (checkReturn == -1) {
            free(graph->vertices);
            free(graph);
            free(player);
            return(0);
        }

        idsTable = malloc(sizeof(unsigned int)*nearVertices);

        for(j=0; j < nearVertices; j++) {

            readReturn = read(fileDescriptor, &idsTable[j], 4);

            // remove late
            //printf("ID that is read is %u\n", idsTable[j]);

            checkReturn = readCheck(readReturn, fileDescriptor);
            if(!checkReturn) {
                graphDelete(graph, i);
                free(graph);
                free(player);
                free(idsTable);

                return(0);
            } else if (checkReturn == -1) {
                graphDelete(graph, i);
                free(graph);
                free(player);
                free(idsTable);
                 
                return(0);
            }
        }

        graph->vertices[i]->edges = malloc(sizeof(edge_t));
        graph->vertices[i]->edges->to = NULL; // to know when it is the first edge

        // sorts the ids and connects them to the edges of graph->vertices[i]
        for(j=0, lowestIdPos = 0; j < nearVertices; j++) {
            for(k=0; k < nearVertices; k++) {
                if(idsTable[k] < idsTable[lowestIdPos]) {
                    lowestIdPos = k;
                }
            }

            nodeCreate(graph->vertices[i], 
            graph->vertices[vertexIdFind(graph, idsTable[lowestIdPos])]);
            idsTable[lowestIdPos] = graph->max_id + 1; // in order for this id to not be selected again
        }

        free(idsTable);
    }

    player->room = graph->vertices[vertexIdFind(graph, playerRoomId)];
    strcpy(player->name, (*oldPlayer)->name);

    if(*oldPlayer != NULL) {
        free(*oldPlayer);
    }

    // checks the game status to avoid double free
    if(*oldGraph != NULL  && gameStatus != GAME_OVER) {
        graphDelete(*oldGraph, (*oldGraph)->num_vertices);
        free(*oldGraph);
    }
    

    // this happens only in the case everything was successful
    *oldGraph = graph;    
    *oldPlayer = player;

    return(1);
}

/*
 * Checks if the file exists and has the right structure
 * Parameters:
 * Returns: 1 if everything is OK , 0 if the file is corruppted and -1 if it fails to open
*/
int gameLoadCheck(player_t *player)
{
    char magicNumberTest[10], filename[NAME_SIZE + 4];
    int fileDescriptor, readReturn, checkReturn;

    filenameCreate(filename, player->name);

    fileDescriptor = open(filename, O_RDONLY , 0644);

    if (fileDescriptor == -1) {
        close(fileDescriptor);
        perror(CORRUPTED_ERROR_MSG);
        printf(OPEN_ERROR_MSG);
        return(-1);
    }

    readReturn = read(fileDescriptor, &magicNumberTest, 10);
    checkReturn = readCheck(readReturn, fileDescriptor);
    if(!checkReturn) {
        return(0);
    } else if (checkReturn == -1) {
        return(0);
    }
    //printf("test is %s and magic num is \n", MAGIC_NUMBER);
    
    if (strncmp(magicNumberTest, MAGIC_NUMBER, 10)) {
        return(0);
    }
    
    lseek(fileDescriptor, 16, SEEK_CUR);

    readReturn = read(fileDescriptor, &magicNumberTest, 10);
    checkReturn = readCheck(readReturn, fileDescriptor);
    if(!checkReturn) {
        return(0);
    } else if (checkReturn == -1) {
        return(0);
    }
    
    if (strncmp(magicNumberTest, MAGIC_NUMBER, 10)) {
        return(0);
    }

    lseek(fileDescriptor, -10, SEEK_END);

    readReturn = read(fileDescriptor, &magicNumberTest, 10);
    checkReturn = readCheck(readReturn, fileDescriptor);
    if(!checkReturn) {
        return(0);
    } else if (checkReturn == -1) {
        return(0);
    }
    
    if (strncmp(magicNumberTest, MAGIC_NUMBER, 10)) {
        return(0);
    }

    return(1);
}

/*
 * Checks if there already exists a file with the same name
 * Parameters: The filename
 * Returns: 1 if the file doesn't exist, 0 if it exists and 2 if there is another type of error
*/
int saveCheck(player_t *player)
{
    int fileDescriptor;
    char filename[NAME_SIZE + 4];

    filenameCreate(filename, player->name);

    fileDescriptor = open(filename, O_RDONLY);

    if(fileDescriptor == - 1) {
        // to make sure the file doesn't exist
        if(errno == ENOENT) {
            return(1);
        }
        else {
            return(2);
        }
    }

    return(0);
}