#include <stdlib.h>
#include "graph.h"

typedef struct edge Edge;

struct square
{
    int p;      //p é a posição  do vértice no tabuleiro
    int weight; //peso do vertice (para transpôr) (ver se faz sentido)
    Square *next;
};
struct graph
{
    int V;   //nº total de vértices
    int L;   //nºde linhas
    int C;   //nºde colunas
    float E; //nº total de arestas (nem sempre está com o valor atualizado)
    Square **adj;
};
struct edge
{
    int v;      //vertice 1 da aresta
    int w;      //vertice 2 da aresta
    int weight; //peso da aresta (ver se faz sentido)
};

Edge *newEdge(int v, int w, int weight)
{
    Edge *newE = (Edge *)malloc(sizeof(struct edge));
    newE->v = v;
    newE->w = w;
    newE->weight = weight;
    return newE;
}

Square *newSquare(int p, int weight)
{
    Square *newS = (Square *)malloc(sizeof(struct square));
    newS->p = p;
    newS->weight = weight;
    newS->next = NULL;
    return newS;
}

/*Square *newS(int p, int weight, Square *next)
{
    Square *newS = (Square *)malloc(sizeof(struct square));
    newS->p = p;
    newS->weight = weight;
    newS->next = next;
    return newS;
} */

Square *addS(Square *headS, Square *newS)
{
    if (headS == NULL)
    {
        headS = newS;
    }
    newS->next = headS->next;
    headS->next = newS;
    return headS;
}

Graph *graphInit(int V, int L, int C, int E)
{
    int v;
    Graph *newG = (Graph *)malloc(sizeof(struct graph));
    newG->V = V;
    newG->L = L;
    newG->C = C;
    newG->E = E;
    newG->adj = (Square **)malloc(V * sizeof(Square *));
    for (v = 0; v < V; v++)
        newG->adj[v] = NULL;
    return newG;
}

int adjacentTile(int tile, int l, int c, int HouV) //HouV- foi realizada soma horizontal ou vertical
{
    if (HouV == 1) //HouV==1 soma horizontal
    {
        if ((tile % 6) == 0)
            return 0; //não é adjacente
    }
    else if (tile < 0 || tile > (l * c - 1))
        return 0; //não é adjacente
    return 1;
}

void graphInsertS(Graph *G, Square *newS)
{
    int v = newS->p;
    Square *headS = G->adj[v];
    G->adj[v] = addS(headS, newS);
    if (adjacentTile(v + 1, G->L, G->C, 1) == 1) //colocar a adjacencia do square na lista
    {
        headS = G->adj[v + 1];
        G->adj[v + 1] = addS(headS, newS);
    }
    if (adjacentTile(v - 1, G->L, G->C, 1) == 1)
    {
        headS = G->adj[v - 1];
        G->adj[v - 1] = addS(headS, newS);
    }
    if (adjacentTile(v + G->C, G->L, G->C, 0) == 1)
    {
        headS = G->adj[v + G->C];
        G->adj[v + G->C] = addS(headS, newS);
    }
    if (adjacentTile(v - G->C, G->L, G->C, 0) == 1)
    {
        headS = G->adj[v - G->C];
        G->adj[v - G->C] = addS(headS, newS);
    }
}

int graphEdges(Edge **edgesArray, Graph *G)
{
    int i, E = 0;
    Square *t;
    for (i = 0; i < G->V; i++)
        for (t = G->adj[i]; t != NULL; t = t->next)
            if (i < t->p)
                edgesArray[E++] = newEdge(i, t->weight, t->p);
    return E;
}
