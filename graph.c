//IGNORAR POR AGORA
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
        return headS;
    }
    newS->next = headS;
    headS = newS;
    return headS;

    //newS->next = headS;
    //return newS;
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

int adjacentTile(int tile, int l, int c, int HouV) //HouV- foi realizada soma(1) ou subtração(2) horizontal
{
    if (tile < 0 || tile > (l * c - 1))
        return 0;  //não é adjacente
    if (HouV == 1) //HouV==1 soma horizontal
    {
        if ((tile % 6) == 0)
            return 0; //não é adjacente
    }
    if (HouV == 2) //HouV==2 subtração horizontal
    {
        if ((tile % 6) == (c - 1))
            return 0; //não é adjacente
    }
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
    if (adjacentTile(v - 1, G->L, G->C, 2) == 1)
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

/*
void printAllGraph(Graph *myGraph)
{
    int i;
    Square **aux;
    aux = myGraph->adj;
    for (i = 0; i < myGraph->V; i++)
    {
        while (aux[i] != NULL)
        {
            printf("bla\n");
        }
    }
}
*/

int insideBoard(int tile, int size)
{
    if (tile < 0 || tile > size)
        return 0; //não está no tabuleiro
    return 1;
}

int checkTile(Graph *myGraph, int tile)
{
    Square *aux = myGraph->adj[tile];
    if (insideBoard(tile, myGraph->V - 1) == 0)
    {
        return -2; // peça fora do tabuleiro
    }
    while (aux != NULL)
    {
        if (aux->p == tile)
            return aux->weight;
        aux = aux->next;
    }
    return 0;
}

int checkAdjacent(Graph *myGraph, int tile, int mode)
{
    Square *aux = myGraph->adj[tile];
    if (insideBoard(tile, myGraph->V - 1) == 0) //verificar se a peça está dentro do tabuleiro
    {
        return -2; // peça fora do tabuleiro
    }
    if (mode == 2) //modo A2-peça branca adjacente
    {
        int i = 0;
        while (aux != NULL)
        {
            aux = aux->next;
            if (aux->p != tile)
                i++;
        }
        if (i == 4) //nenhuma adjacente é branca
            return 0;
        /*verificar se tile é uma peça de canto*/
        if (tile == 0 || tile == (myGraph->C + 1) || tile == (myGraph->C * (myGraph->L - 1)) || tile == (myGraph->C * myGraph->L - 1))
        {
            if (i == 2) //peça de canto com 2 adjacencias (não brancas) => que não tem adjacencias brancas
                return 0;
            return 1;
        }
        /*verificar se tile é uma peça lateral*/
        if ((tile % myGraph->C) == 0 || (tile % myGraph->C) == (myGraph->C - 1) || tile < myGraph->C || (tile < (myGraph->V - 1) && (tile > (myGraph->C * (myGraph->L - 1)))))
        {
            if (i == 3) //peça laterak com 3 adjacencias (não brancas) => que não tem adjacencias brancas
                return 0;
            return 1;
        }
        return 1;
    }
    if (mode == 3) //modo A3-peça cinzenta adjacente
    {
        while (aux != NULL)
        {
            if ((aux->p != tile) && (aux->weight > 0))
                return 1; //encontrou-se uma peça adjacencente cinzenta
            aux = aux->next;
        }
        return 0;
    }
    if (mode == 4) //modo A4-peça preta adjacente
    {
        while (aux != NULL)
        {
            if ((aux->p != tile) && (aux->weight == -1))
                return 1; //encontrou-se uma peça adjacencente preta
            aux = aux->next;
        }
        return 0;
    }
    return 9; //erro
}