#include "graph.h"
#include "stdio.h"
#include "stdlib.h"

#include "libhw3.h"
#include "random.h"

void nodeAdd(graph_t *graph, contents_t contents)
{
    //unsigned int verticeNum1, verticeNum2;
    //edge_t *currEdge, *newEdge1, *newEdge2;

    /*
    newEdge1 = malloc(sizeof(edge_t));
    newEdge2 = malloc(sizeof(edge_t));
    */
    
    graph->max_id++;
    graph->num_vertices++;

    graph->vertices = realloc(graph->vertices, graph->num_vertices*sizeof(vertex_t*));

    graph->vertices[graph->num_vertices - 1] = malloc(sizeof(vertex_t));

    graph->vertices[graph->num_vertices - 1]->id = graph->max_id;
    graph->vertices[graph->num_vertices - 1]->contents = contents;

    graph->vertices[graph->num_vertices - 1]->edges = malloc(sizeof(edge_t));
    
    
    //graph->vertices[graph->num_vertices - 1]->edges->next = malloc(sizeof(edge_t));

    graph->vertices[graph->num_vertices - 1]->edges->to = NULL;
    
    /*
    verticeNum1 = get_rand_range(0, graph->num_vertices - 1);

    verticeNum2 = get_rand_range(0, graph->num_vertices - 1);
    */

    //nodesConnect(graph->vertices[graph->num_vertices - 1], graph->vertices[verticeNum1]);
    //nodesConnect(graph->vertices[graph->num_vertices - 1], graph->vertices[verticeNum2]);
    
    /*
    graph->vertices[graph->num_vertices - 1]->edges->to = graph->vertices[verticeNum1];
    graph->vertices[graph->num_vertices - 1]->edges->next->to = graph->vertices[verticeNum2];
    graph->vertices[graph->num_vertices - 1]->edges->next->next = NULL;

    for (currEdge =  graph->vertices[verticeNum1]->edges; currEdge->next != NULL; 
    currEdge = currEdge->next);
    currEdge->next = newEdge1;
    newEdge1->next = NULL;
    newEdge1->to = graph->vertices[graph->num_vertices - 1];

    for (currEdge =  graph->vertices[verticeNum2]->edges; currEdge->next != NULL; 
    currEdge = currEdge->next);
    currEdge->next = newEdge2;
    newEdge2->next = NULL;
    newEdge2->to = graph->vertices[graph->num_vertices - 1];
    */
}

/*
 * Connects the two vertices given by creating a new edge in the end of each
 * Parameters: The two vertices
 * Returns: Nothing
*/
void nodesConnect(vertex_t *node1, vertex_t *node2)
{
    edge_t *currEdge, *newEdge1, *newEdge2;

    newEdge1 = malloc(sizeof(edge_t));
    newEdge2 = malloc(sizeof(edge_t));

    // in case the edge is completely new
    if (node1->edges->to == NULL) {
        free(newEdge1);
        node1->edges->to = node2;
        node1->edges->next = NULL;
    } else {
        for (currEdge = node1->edges; currEdge->next != NULL; currEdge = currEdge->next);
        currEdge->next = newEdge1;
        newEdge1->next = NULL;
        newEdge1->to = node2;
    }

    // in case the edge is completely new
    if (node2->edges->to == NULL) {
        free(newEdge2);
        node2->edges->to = node2;
        node2->edges->next = NULL;
    } else {
        for (currEdge = node2->edges; currEdge->next != NULL; currEdge = currEdge->next);
        currEdge->next = newEdge2;
        newEdge2->next = NULL;
        newEdge2->to = node1;
    }
}

/*
 * Creats a new edge in the node vertex that points to the target vertex
 * Parameters: The two vertices
 * Returns: Nothing
*/
void nodeCreate(vertex_t *node, vertex_t *target)
{
    edge_t *currEdge, *newEdge;

    newEdge = malloc(sizeof(edge_t));

    // in case the edge is completely new
    if (node->edges->to == NULL) {
        free(newEdge);
        node->edges->to = target;
        node->edges->next = NULL;
    } else {
        for (currEdge = node->edges; currEdge->next != NULL; currEdge = currEdge->next);
        currEdge->next = newEdge;
        newEdge->next = NULL;
        newEdge->to = target;
    }
}

void nodesDisconnect(vertex_t *node1, vertex_t *node2, graph_t *graph)
{
    edge_t *currEdge, *prevEdge = NULL;

    //print_graph(graph);
    
    for (currEdge = node1->edges, prevEdge = NULL; currEdge->next != NULL; 
    prevEdge = currEdge, currEdge = currEdge->next) {
        
        if (currEdge->to == node2) {
            
            if(currEdge == node1->edges) {
                node1->edges = currEdge->next;
                free(currEdge);
                break;
            }

            prevEdge->next = currEdge->next;
            free(currEdge);
            break;
        }
    }

    // in case there is only one edge (node1->edges)
    if(prevEdge == NULL && node1->edges->to == node2) {
        free(node1->edges);
    }
    
    // to check the last edge too
    if ((prevEdge != NULL) && (prevEdge->next->next == NULL) && (prevEdge->next->to == node2)) {
        prevEdge->next = NULL; //currEdge->next;
        free(currEdge);
    }

    //other node

    for (currEdge = node2->edges, prevEdge = NULL; currEdge->next != NULL; 
    prevEdge = currEdge, currEdge = currEdge->next) {
        if (currEdge->to == node1) {

            if(currEdge == node2->edges) {
                node2->edges = currEdge->next;
                free(currEdge);
                break;
            }

            prevEdge->next = currEdge->next;
            free(currEdge);
            break;
        }
    }

    // in case there is only one edge (node2->edges)
    if(prevEdge == NULL && node2->edges->to == node1) {
        free(node2->edges);

        //?
        *node2 = *graph->vertices[graph->num_vertices - 1];

        graph->num_vertices--;

        graph->vertices = realloc(graph->vertices, sizeof(vertex_t)*graph->num_vertices);
    } 
    
    // to check the last edge too
    if ((prevEdge != NULL) && (prevEdge->next->next == NULL) && (prevEdge->next->to == node1)) {
        prevEdge->next = NULL; //currEdge->next;
        free(currEdge);
    }
}

void nodeDelete(graph_t *graph ,vertex_t *node, int vertexPosDelete)
{
    edge_t *currEdge, *prevEdge;

    prevEdge = node->edges;
    //print_graph(graph);
    if (node->edges->next != NULL) {
        for (currEdge = prevEdge->next; currEdge->next != NULL; 
        prevEdge = currEdge, currEdge = currEdge->next) {
            nodesDisconnect(node, prevEdge->to, graph);
        }

        nodesDisconnect(node, prevEdge->to, graph);
        nodesDisconnect(node, currEdge->to, graph);
    } else {
        nodesDisconnect(node, prevEdge->to, graph);
    }

    free(graph->vertices[vertexPosDelete - 1]);
    
    graph->vertices[vertexPosDelete - 1] = graph->vertices[graph->num_vertices - 1];
    graph->num_vertices--;

    graph->vertices = realloc(graph->vertices, sizeof(vertex_t*)*graph->num_vertices);
    
}

void graphDelete(graph_t *graph, int numVertices)
{
    int i;
    edge_t *currEdge, *prevEdge;
    
    for(i=0; i < numVertices; i++) {
        prevEdge = graph->vertices[i]->edges;  
        
        if(prevEdge->next != NULL) {
            for(currEdge = prevEdge->next; 
            currEdge->next != NULL; prevEdge = currEdge, currEdge = currEdge->next) {
                free(prevEdge);
            }
            free(currEdge);
        }

        free(prevEdge);
    }

    // seperately because of the use in gameLoad error return
    for(i=0; i < graph->num_vertices; i++) {
        free(graph->vertices[i]);
    }

    free(graph->vertices);    
    //free(graph);
}

int vertexIdFind(graph_t *graph, unsigned int id)
{
    int i;

    for(i=0; i < graph->num_vertices; i++) {
        if(graph->vertices[i]->id == id) {
            return(i);
        }
    }

    printf("Search for id failed. ID is: %u. Max ID is: %u. i is equal to %d\n", id,
    graph->max_id, i);
    return(-1);
}

unsigned int nearVerticesCount(vertex_t *vertex)
{
    edge_t *currEdge;
    unsigned int verticesAmount = 1;

    for(currEdge = vertex->edges; currEdge->next != NULL; currEdge = currEdge->next) {
        verticesAmount++;
    }
    
    return(verticesAmount);
}

void nearVerticesPrint(vertex_t *vertex)
{
    edge_t *currEdge;
    int verticesAmount, i, j, lowerIDPosition;
    unsigned int *idsArray, *usedIDs;

    verticesAmount = nearVerticesCount(vertex);    

    idsArray = malloc(sizeof(unsigned int)*verticesAmount);
    usedIDs = malloc(sizeof(unsigned int)*verticesAmount);
    
    for (currEdge = vertex->edges, i = 0; i < verticesAmount; currEdge = currEdge->next, i++) {
        idsArray[i] = currEdge->to->id;
        usedIDs[i]=0;
    }

    for (i=0, lowerIDPosition = 0; i < verticesAmount; i++) {
        for (j = 0; j < verticesAmount; j++) {
            if (((idsArray[j] < idsArray[lowerIDPosition]) && (usedIDs[j] == 0)) 
            || ((usedIDs[lowerIDPosition] == 1) && (usedIDs[j] == 0))) {
                lowerIDPosition = j;
            }
        }

        printf(" %u", idsArray[lowerIDPosition]);
        usedIDs[lowerIDPosition] = 1;
    }

    printf("\n");

    free(idsArray);
    free(usedIDs);
}

int vertexContentSearch(contents_t contents, vertex_t *vertex)
{
    edge_t *currEdge;
    int contentsFound = 0;

    for (currEdge = vertex->edges; currEdge->next != NULL; currEdge = currEdge->next) {
        if (currEdge->to->contents == contents) {
            contentsFound = 1;
        }
    }
    if (currEdge->to->contents == contents) {
        contentsFound = 1;
    }
    return(contentsFound);
}

int vertexIdSearch(unsigned int id, vertex_t *vertex)
{
    edge_t *currEdge;
    int idFound = 0;

    for (currEdge = vertex->edges; currEdge->next != NULL; currEdge = currEdge->next) {
        if (currEdge->to->id == id) {
            idFound = 1;
        }
    }
    if (currEdge->to->id == id) {
        idFound = 1;
    }
    return(idFound);
}