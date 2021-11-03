#ifndef __grafo2__
#define __grafo2__

typedef struct edge Edge;
Edge *newEdge(int v, int w, int p, int weight); //função que faz uma aresta entre duas salas
typedef struct graph Graph;
Graph *graphInit(int);              //cria um grafo com o número de vertices
void graphInsertE(Graph *, Edge *); //insere uma aresta no grafo
void graphDestroy(Graph *);

#endif /* __grafo2__ */
