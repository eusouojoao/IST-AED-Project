#ifndef __grafo2__
#define __grafo2__

typedef struct edge Edge;
Edge *newEdge(int v, int w, int p, int weight); //função que faz uma aresta entre duas salas
typedef struct graph Graph;
Graph *graphInit(int);              //cria um grafo com um numero de vertices
void graphInsertE(Graph *, Edge *); //insere uma aresta no grafo
//int graphEdges(Edge **, Graph *); //diz o numero de quadrados e coloca as arestas num vetor
void graphDestroy(Graph *);
void printGraph(Graph *);

#endif /* __grafo2__ */
