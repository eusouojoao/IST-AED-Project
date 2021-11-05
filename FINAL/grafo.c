#include <stdlib.h>
#include "grafo.h"
#include "Item.h"

typedef struct room Room;

struct room
{
    int n;      //número da sala
    int weight; //peso associado
    int p;      //posição da parede (que tem o peso "weight")
    Room *next;
};

/* estrutura principal do grafo */
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

/**
 * @brief Função que queria uma nova sala na lista de adjacencias
 * @note   
 * @param  n: número da sala adjacente
 * @param  *next: apontador para o primeiro elemento da lista de adjacencias
 * @param  weight: peso da parede a transpôr para entrar na sala referida
 * @param  p: posição da parede a transpôr para entrar na sala referida
 * @retval apontador para a sala criada
 */
Room *newRoom(int n, Room *next, int weight, int p)
{
    Room *newR = (Room *)malloc(sizeof(struct room));
    newR->n = n;
    newR->p = p;
    newR->weight = weight;
    newR->next = next; //a nova sala criada passa a ser a primeira da lista de adjacências
    return newR;
}

/**
 * @brief  Função que queria uma aresta entre duas salas
 * @note   
 * @param  v: numero da sala 1
 * @param  w: numero da sala 2
 * @param  p: posição da parede a partir para transpôr da sala e para a sala 2
 * @param  weight: peso da parede a partir para transpôr da sala e para a sala 2
 * @retval apontador para a aresta criada
 */
Edge *newEdge(int v, int w, int p, int weight)
{
    Edge *newE = (Edge *)malloc(sizeof(struct edge));
    newE->p = p;
    newE->v = v;
    newE->w = w;
    newE->weight = weight;
    newE->p = p;
    return newE;
}

/**
 * @brief  Funçaõ que inicializa a lista de adjancências com uma certa dimensão
 * @note   
 * @param  V: dimensão da lista de adjacencias
 * @retval apontador para o grafo criado
 */
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

/**
 * @brief  Função que recebe uma aresta e insere-a no grafo
 * @note   a função, não insere a aresta no grafo, caso já exista alguma aresta de menor peso no grafo
 * @param  *G: apontador para o grafo
 * @param  *newE: aresta que contem a informação a ser inserida no grafo
 * @retval None
 */
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
            free(newE);
            return;
        }
        aux = aux->next;
    }
    //caso a aresta não exista, criar uma nova
    G->adj[v] = newRoom(w, G->adj[v], weight, p);
    G->adj[w] = newRoom(v, G->adj[w], weight, p);
    G->E++;
    free(newE);
    return;
}

/**
 * @brief  Função que apaga o grafo
 * @note   
 * @param  *myGraph: apontador para o grafo
 * @retval None
 */
void graphDestroy(Graph *myGraph)
{
    Room *del = NULL;
    Room *aux = NULL;
    for (int j = 0; j < myGraph->V; j++)
    {
        aux = myGraph->adj[j];
        while (aux != NULL)
        {
            del = aux;
            aux = aux->next;
            free(del);
        }
    }

    free(myGraph->adj);
    free(myGraph);
}

/**
 * @brief  Função que retorna a lista de adjacentes ao vertice v
 * @note   
 * @param  *G: apontador para o grafo
 * @param  **t: apontador para o apontador da sala atual
 * @param  v: vertice para o qual queremos a lista 
 * @retval None
 */
void assignLista( Graph *G, void **t, int v )
{
    (*t) =  (void *) G->adj[v];
    return;
}

/**
 * @brief  Função que retorna o proximo elemento da lista
 * @note   
 * @param  **t: apontador para o apontador da sala atual
 * @retval None
 */
void ListaNext ( void **t )
{
    (*t) = (void *) ((Room *)(*t))->next;
    return;
}

/**
 * @brief  Retorna a sala
 * @note   
 * @param  *t: apontador para a sala atual na lista de adjacencias
 * @retval número da sala atual na lista de adjacencias
 */
int getSala ( void *t )
{
    return ((Room *)t)->n;
}

/**
 * @brief  Retorna o peso/distancia para a sala 
 * @note   
 * @param  *t: apontador para a sala atual na lista de adjacencias
 * @retval distancia/peso para a sala
 */
int getDist ( void *t )
{
    return ((Room *)t)->weight;
}
/**
 * @brief  Retorna o número de vertices no grafo
 * @note   
 * @param  *G: apontador para o grafo
 * @retval numero de vertices (int)
 */
int getVertices ( Graph *G )
{
    return G->V;
}

/**
 * @brief  Retorna a parede
 * @note   
 * @param  *t: apontador para a sala atual na lista de adjacencias
 * @retval coordenada unidimensional da parede
 */
int getWall ( void *t )
{
    return ((Room *)t)->p;
}
