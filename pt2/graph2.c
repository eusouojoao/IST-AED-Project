#include <stdlib.h>
#include <stdio.h>
#include "graph2.h"

typedef struct room Room;

struct room
{
    int n;      //numero da sala
    int weight; //peso associado
    int p;      //posição da parede (que tem o peso w)
    Room *next;
};
struct graph
{
    int V; //nº total de salas
    int E; //nº total de arestas
    Room **adj;
};
struct edge
{
    int v;      //vertice 1 da aresta
    int w;      //vertice 2 da aresta
    int p;      //posição de custo minimo no tabuleiro
    int weight; //peso da aresta
};

Room *newRoom(int n, Room *next, int weight, int p)
{
    Room *newR = (Room *)malloc(sizeof(struct room));
    newR->n = n;
    newR->p = p;
    newR->weight = weight;
    newR->next = next;
    return newR;
}

Edge *newEdge(v, w, p, weight)
{
    Edge *newE = (Edge *)malloc(sizeof(struct edge));
    newE->p = p;
    newE->v = v;
    newE->w = w;
    newE->weight = weight;
    newE->p = p;
    return newE;
}

Graph *graphInit(int V)
{
    int v;
    Graph *newG = (Graph *)malloc(sizeof(struct graph));
    newG->V = V;
    newG->E = 0;
    newG->adj = (Room **)malloc(V * sizeof(Room *));
    for (v = 0; v < V; v++)
        newG->adj[v] = NULL;
    return newG;
}

void graphInsertE(Graph *G, Edge *newE)
{
    int v = newE->v;
    int w = newE->w;
    int weight = newE->weight;
    int p = newE->p;
    Room *aux = G->adj[v], *aux2 = G->adj[w];
    while (aux != NULL) //procurar no grafo se a aresta já existe
    {
        if (aux->n == w)
        {
            if (aux->weight > weight) //substituir a aresta caso, se encontre uma de menor custo
            {
                aux->p = p;
                aux->weight = weight;
                while (aux2 != NULL)
                {
                    if (aux2->n == v)
                    {
                        aux2->p = p;
                        aux2->weight = weight;
                        break;
                    }
                    aux2 = aux2->next;
                }
                free(newE);
                return;
            }
            return;
        }
        aux = aux->next;
    }
    //caso a aresta não exista, criar uma nova
    G->adj[v] = newRoom(w, G->adj[v], weight, p);
    G->adj[w] = newRoom(v, G->adj[w], weight, p);
    G->E++; //Confirmar isto
    free(newE);
    return;
}

void graphDestroy(Graph *myGraph)
{
    int v = 0, V = myGraph->V;
    free(myGraph->adj);
    free(myGraph);
}

void printGraph(Graph *G)
{
    int i, n_rooms = G->V;
    for (i = 0; i < n_rooms; i++)
    {
        //  if (G->adj[i] != NULL)
        while (G->adj[i] != NULL)
        {
            printf("A sala %d está unida à sala %d com o custo %d na posição %d\n", i, G->adj[i]->n, G->adj[i]->weight, G->adj[i]->p);
            G->adj[i] = G->adj[i]->next;
        }
    }
}