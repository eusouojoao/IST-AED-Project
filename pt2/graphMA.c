#include <stdlib.h>
#include "graph2.h"

struct graph
{
    int V; //nº total de salas
    int E; //nº total de arestas
    int **adj;
};

struct edge
{
    int v;      //vertice 1 da aresta
    int w;      //vertice 2 da aresta
    int p;      //posição de custo minimo no tabuleiro
    int weight; //peso da aresta
};

Graph *graphInit(int V)
{
    Graph *newG = (Graph *)malloc(sizeof(struct graph));
    newG->V = V;
    newG->E = 0;
    newG->adj = matrixInt(V, V, 0);
    return newG;
}

void graphInsertE(Graph *G, Edge *newE)
{
    int v = newE->p;
    int v = newE->v;
    int w = newE->w;
    if (G->adj[v][w] == 0)
        G->E++;
    G->adj[v][w] = w;
    G->adj[w][v] = w;
}

int GRAPHedges(Edge *a[], Graph *G)
{
    int v, w, E = 0;
    for (v = 0; v < G->V; v++)
        for (w = v + 1; w < G->V; w++)
            if (G->adj[v][w] == 1)
                a[E++] = EDGE(v, w);
    return E;
}