#ifndef __grafo__
#define __grafo__

typedef struct edge Edge;
Edge *newEdge(int v, int w, int p, int weight); //função que faz uma aresta entre duas salas
typedef struct graph Graph;
Graph *graphInit(int);              //cria um grafo com o número de vertices
void graphInsertE(Graph *, Edge *); //insere uma aresta no grafo
void graphDestroy(Graph *);
void assignLista(Graph *G, void **t, int v);
void ListaNext(void **t);
int getSala(void *t);
int getDist(void *t);
int getVertices(Graph *G);
int getWall(void *t);
void mergeRooms(Graph *G, int room1, int room2);
void printGraph(Graph *G);

#endif /* __grafo__ */
