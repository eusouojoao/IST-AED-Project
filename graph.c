#include <stdlib.h>
#include "graph.h"

typedef struct square square;
struct square
{
    int p;      //p é a posição  do vértice no tabuleiro
    int weight; //peso do vertice (para transpôr) (ver se faz sentido)
    square *next;
};
struct graph
{
    int V;   //nº total de vértices
    float E; //nº total de arestas (nem sempre está com o valor atualizado)
    square **adj;
};
struct edge
{
    int v;      //vertice 1 da aresta
    int w;      //vertice 2 da aresta
    int weight; //peso da aresta (ver se faz sentido)
};

square *newSquare(int p, int weight, square *next)
{
    square *newS = (square *)malloc(sizeof(struct square));
    newS->p = p;
    newS->weight = weight;
    newS->next = next;
    return newS;
}

Graph *GraphInit(int V)
{
    int v;
    Graph *newG = (Graph *)malloc(sizeof(struct graph));
    newG->V = V;
    newG->E = 0;
    newG->adj = (square **)malloc(V * sizeof(square *));
    for (v = 0; v < V; v++)
        newG->adj[v] = NULL;
    return newG;
}

void GraphInsertS(Graph *G, Edge *e)
{
    int v = e->v, w = e->w, weight = e->weight;
    G->adj[v] = newSquare(w, weight, G->adj[v]); //tamos a supôr que não são inseridas variáveis repetidas
    G->adj[w] = newSquare(v, weight, G->adj[w]);
    G->E++;
}

int GraphEdges(Edge **EdgesArray, Graph *G)
{
    int i, E = 0;
    square *t;
    for (i = 0; i < G->V; i++)
        for (t = G->adj; t != NULL; t = t->next)
            if (i < t->p)
                EdgesArray[E++] = newEdge(i, t->weight, t->p);
    return E;
}