#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "acervo.h"
#include "graph2.h"

#define P (dist[v] + t->weight)
static int *dist;
static int *parent;

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

/**
 * @brief  
 * @note   
 * @param  a: 
 * @param  b: 
 * @retval 
 */
int comparaRooms(int a, int b)
{
    if (dist[a] > dist[b])
        return 1;
    if (dist[a] < dist[b])
        return -1;
    else
        return 0;
}

/**
 * @brief  
 * @note   
 * @param  *G: 
 * @retval None
 */
void algoritmo(Graph *G)
{
    int v, w;
    Room *t;
    acervo *A = (acervo *)malloc(sizeof(acervo));
    if (A == NULL)
        exit(0);

    dist = (int *)malloc(G->V * sizeof(int));
    if (dist == NULL)
        exit(0);

    parent = (int *)malloc(G->V * sizeof(int));
    if (parent == NULL)
        exit(0);

    acervoInit(A, comparaRooms, G->V);

    for (v = 0; v < G->V; v++)
    {
        dist[v] = INT_MAX;
        parent[v] = -1;
    }

    acervoAdd(A, 0);
    dist[0] = 0;

    while (!acervoEmpty(A))
    {
        if (dist[(v = acervoGet(A))] != INT_MAX)
            for (t = G->adj[v]; t != NULL; t = t->next)
                if (dist[(w = t->n)] > P)
                {
                    dist[w] = P;
                    //if (parent[w] == -1)
                    acervoAdd(A, w);
                    parent[w] = v;
                }
    }

    free(A->acervoArray);
    free(A);
    return;
}

/**
 * @brief  
 * @note   
 * @param  coord: 
 * @param  colunas: 
 * @retval 
 */
int Linha(int coord, int colunas)
{
    return ((int)ceil((float)(coord + 1) / (float)colunas));
}

/**
 * @brief  
 * @note   
 * @param  coord: 
 * @param  Linha: 
 * @param  colunas: 
 * @retval 
 */
int Coluna(int coord, int Linha, int colunas)
{
    return (coord + 1 - ((Linha - 1) * colunas));
}

/**
 * @brief  
 * @note   
 * @param  *list: 
 * @param  p: 
 * @param  *wall: 
 * @param  *weight: 
 * @retval None
 */
void getParede(Room *list, int p, int *wall, int *weight)
{
    Room *aux = list;
    while (aux != NULL && aux->n != p)
        aux = aux->next;

    if (aux == NULL)
        exit(0);

    *wall = aux->p;
    *weight = aux->weight;

    return;
}

/**
 * @brief  
 * @note   
 * @param  *fp: 
 * @param  *G: 
 * @param  p: 
 * @param  colunas: 
 * @retval None
 */
void printRecursivo(FILE *fp, Graph *G, int p, int colunas)
{
    int wall, weight;
    int linha, coluna;

    if (parent[p] == 0)
    {

        getParede(G->adj[0], p, &wall, &weight);
        linha = Linha(wall, colunas);
        coluna = Coluna(wall, linha, colunas);
        fprintf(fp, "%d %d %d\n", linha, coluna, weight);

        return;
    }

    printRecursivo(fp, G, parent[p], colunas);

    getParede(G->adj[parent[p]], p, &wall, &weight);
    linha = Linha(wall, colunas);
    coluna = Coluna(wall, linha, colunas);
    fprintf(fp, "%d %d %d\n", linha, coluna, weight);

    return;
}

/**
 * @brief  
 * @note   
 * @param  *output: 
 * @param  *G: 
 * @param  tesouroRoom: 
 * @param  colunas: 
 * @param  first: 
 * @retval None
 */
void writeSolution(char *output, Graph *G, int tesouroRoom, int colunas, bool first)
{
    int i = tesouroRoom;
    int distance = dist[tesouroRoom];
    int cnt = 0;

    FILE *fp = fopen(output, "a+");
    if (fp == NULL)
        return;

    if (!first)
    {
        fprintf(fp, "\n\n");
    }

    if (distance != INT_MAX)
    {
        fprintf(fp, "%d\n", distance);

        for (cnt = 0; parent[i] != -1; i = parent[i], cnt++)
            ;

        if (cnt == 0)
        {
            fclose(fp);
            free(dist);
            free(parent);
            return;
        }

        fprintf(fp, "%d\n", cnt);
        printRecursivo(fp, G, tesouroRoom, colunas);
    }
    else
    {
        fprintf(fp, "-1\n");
    }

    fclose(fp);
    free(dist);
    free(parent);
    return;
}
