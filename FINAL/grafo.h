#ifndef __grafo__
#define __grafo__

typedef struct edge Edge;
Edge *newEdge(int v, int w, int p, int weight);
typedef struct graph Graph;
Graph *graphInit(int);
void graphInsertE(Graph *, Edge *);
void graphDestroy(Graph *);
void assignLista(Graph *G, void **t, int v);
void listaNext(void **t);
int getSala(void *t);
int getDist(void *t);
int getVertices(Graph *G);
int getWall(void *t);

#endif /* __grafo__ */
