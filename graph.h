#ifndef _HW3_GRAPH
#define _HW3_GRAPH

typedef enum {
	EMPTY, PORTAL, MINE, ARROW, DRAGON
} contents_t;

struct _edge;

typedef struct  _vertex {
	unsigned int id; // unique id
	contents_t contents;
	struct _edge *edges; // a vertex connects to a list of edges
} vertex_t;

typedef struct _edge {
	vertex_t *to;  // the other adjacent vertex
	struct _edge *next;
} edge_t;
	
typedef struct {
	unsigned int max_id; // maximum vertex id given at any time
	unsigned int num_vertices;
	vertex_t **vertices;  // the graph is a dynamic array of pointers to vertices
} graph_t;

void nodeAdd(graph_t *graph, contents_t contents);

void nodesConnect(vertex_t *node1, vertex_t *node2);

/*
 * Creats a new edge in the node vertex that points to the target vertex
 * Parameters: The two vertices
 * Returns: Nothing
*/
void nodeCreate(vertex_t *node, vertex_t *target);

void nodesDisconnect(vertex_t *node1, vertex_t *node2, graph_t *graph); // remome the graph later

void nodeDelete(graph_t *graph ,vertex_t *node, int vertexPosDelete);

void graphDelete(graph_t *graph, int numVertices);

/*
 * Searches the graph for the position of the vertex with the given id
 * Parameters: The graph and the id
 * Returns: The position on the array of vertices for the given id or -1 if it failed
*/
int vertexIdFind(graph_t *graph, unsigned int id);

/*
 * Counts how many vertices are near the vertex given
 * Parameters: The vertex whose neighbours we want to count
 * Returns: The amount of vertices near it 
*/
unsigned int nearVerticesCount(vertex_t *vertex);

/*
 * Prints the ids of the vertices near the vertex given in asceding order
 * Parameters: The vertex whose neighbour's ids we want to print
 * Returns: Nothing
*/
void nearVerticesPrint(vertex_t *vertex);

/*
 * Searches the given vertex's neighbours for the contents_t that is given
 * Parameters: The contents_t we are searching for and the vertex
 * Returns: 0 if it wasn't found and 1 if it was found
*/
int vertexContentSearch(contents_t contents, vertex_t *vertex);

/*
 * Searches the given vertex's neighbours for the id that is given
 * Parameters: The id we are searching for and the vertex
 * Returns: 0 if it wasn't found and 1 if it was found
*/
int vertexIdSearch(unsigned int id, vertex_t *vertex);

#endif
