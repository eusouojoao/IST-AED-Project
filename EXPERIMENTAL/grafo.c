#include <stdlib.h>
#include <stdio.h>
#include "grafo.h"
#include "item.h"

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
void assignLista(Graph *G, void **t, int v)
{
    (*t) = (void *)G->adj[v];
    return;
}

/**
 * @brief  Função que retorna o proximo elemento da lista
 * @note   
 * @param  **t: apontador para o apontador da sala atual
 * @retval None
 */
void ListaNext(void **t)
{
    (*t) = (void *)((Room *)(*t))->next;
    return;
}

/**
 * @brief  Retorna a sala
 * @note   
 * @param  *t: apontador para a sala atual na lista de adjacencias
 * @retval número da sala atual na lista de adjacencias
 */
int getSala(void *t)
{
    return ((Room *)t)->n;
}

/**
 * @brief  Retorna o peso/distancia para a sala 
 * @note   
 * @param  *t: apontador para a sala atual na lista de adjacencias
 * @retval distancia/peso para a sala
 */
int getDist(void *t)
{
    return ((Room *)t)->weight;
}

/**
 * @brief  Retorna o número de vertices no grafo
 * @note   
 * @param  *G: apontador para o grafo
 * @retval numero de vertices (int)
 */
int getVertices(Graph *G)
{
    return G->V;
}

/**
 * @brief  Retorna a parede
 * @note   
 * @param  *t: apontador para a sala atual na lista de adjacencias
 * @retval coordenada unidimensional da parede
 */
int getWall(void *t)
{
    return ((Room *)t)->p;
}

/**
 * @brief  Função que junta 2 salas
 * @note   É necessário copiar todas as ligações da sala 2 e depois eleminá-las 
 * @param  *G: apontador para o grafo
 * @param  room1: sala 1
 * @param  room2: sala 2 (sala a sere eliminada)
 * @retval None
 */
void mergeRooms(Graph *G, int room1, int room2)
{
    int adjRoom, adjP, adjWeight;
    Room *aux = G->adj[room2], *auxPrev = G->adj[room2], *aux2 = NULL, *aux2Prev = NULL;
    /* percorrer todas as salas (nós) adjacentes à sala 2 */
    while (aux != NULL)
    {
        adjRoom = aux->n;
        adjP = aux->p;
        adjWeight = aux->weight;
        /* percorrer todas as salas (nós) adjacentes à sala adjacente à sala 2 (adjRoom), até encontrar a sala 2 ou a sala 1 */
        aux2 = G->adj[adjRoom];
        aux2Prev = G->adj[adjRoom];
        while ((aux2->n != room2) && (aux2->n != room1))
        {
            aux2Prev = aux2;
            aux2 = aux2->next;
        }
        /* caso se encontre primeiro a sala 1 como adjacente */
        if (aux2->n == room1)
        {
            /* Caso o custo da adjacencia, sala "adjRoom" - sala 1, seja maior
            que o custo da adjacencia, sala 2 - sala "adjRoom", trocar o peso e
            posição da parede. 
            É necessário efetuar esta troca pois todas as adjacencias da sala 2
            vão desaparecer*/
            if (aux2->weight > adjWeight)
            {
                aux2->weight = adjWeight;
                aux2->p = adjP;
            }
            /* procurar a sala 2 (ainda no conjunto de adjacencias da sala "adjRoom") e eliminar essa adjacencia*/
            while (aux2->n != room2)
            {
                aux2Prev = aux2;
                aux2 = aux2->next;
            }
            aux2Prev->next = aux2->next;
            free(aux2);
            if (aux2 == NULL)
                G->adj[adjRoom] = NULL;
        }
        /* caso se encontre primeiro a sala 2 como adjacente */
        else if (aux2->n == room2)
        {
            /* eliminar a adjacencia, sala "adjRoom" - sala 2 */
            aux2Prev->next = aux2->next;
            free(aux2);
            aux2 = aux2Prev->next;
            if (aux2 == NULL)
                G->adj[adjRoom] = NULL;
            /* procurar a sala 1 (ainda no conjunto de adjacencias da sala "adjRoom") */
            if (aux2 != NULL)
                while (aux2->n != room1)
                {
                    aux2 = aux2->next;
                    if (aux2 == NULL)
                        break;
                }
            if (adjRoom == room1) // Ignorar caso a sala 1 e a sala adjacente à 2 forem a mesma
                ;
            /* caso não exista adjacencia entre a, sala "adjRoom" - sala 2,
               criar uma nova sala entre a sala "adjRoom" e sala 1. 
               É necessário fazer isto pois a sala 2 vai ser eliminada e as
               adjacencias da sala 2 devem passar para a sala 1*/
            else if (aux2 == NULL)
            {
                aux2 = newRoom(room1, aux2, adjWeight, adjP);
            }
            /* Caso o custo da adjacencia, sala "adjRoom" - sala 1, seja maior que
            o custo da adjacencia, sala 2 - sala "adjRoom", trocar o peso e
            posição da parede. */
            else if (aux2->weight > adjWeight)
            {
                aux2->weight = adjWeight;
                aux2->p = adjP;
            }
        }

        /* percorrer todas as salas (nós) adjacentes à sala 1, até encontrar a sala adjacente à sala 2 (adjRoom) */
        aux2 = G->adj[room1];
        if (aux2 != NULL)
            while (aux2->n != adjRoom)
            {
                aux2 = aux2->next;
                if (aux2 == NULL)
                    break;
            }
        if (adjRoom == room1) // Ignorar caso a sala 1 e a sala adjacente à 2 forem a mesma
            ;
        /*  caso não exista adjacencia entre a, sala 1 - sala "adjRoom", criar
            uma nova sala entre a sala 1 e sala "adjRoom". É necessário fazer
            isto pois a sala 2 vai ser eliminada e as adjacencias da sala 2
            devem passar para a sala 1*/
        else if (aux2 == NULL)
        {
            aux2 = newRoom(adjRoom, aux2, adjWeight, adjP);
        }
        /*  Caso o custo da adjacencia, sala 1 - sala "adjRoom", seja maior que
            o custo da adjacencia, sala 2 - sala "adjRoom", trocar o peso e
            posição da parede. */
        else if (aux2->weight > adjWeight)
        {
            aux2->weight = adjWeight;
            aux2->p = adjP;
        }

        /* eliminar a adjacencia, sala 2 - sala "adjRoom" */
        auxPrev->next = aux->next;
        free(aux);
        aux = auxPrev->next;
        if (aux == NULL)
            G->adj[room2] = NULL;
    }
    return;
}

void printGraph(Graph *G)
{
    int i, n_rooms = G->V;
    for (i = 0; i < n_rooms; i++)
    {
        while (G->adj[i] != NULL)
        {
            printf("A sala %d está unida à sala %d com o custo %d na posição %d\n", i, G->adj[i]->n, G->adj[i]->weight, G->adj[i]->p);
            G->adj[i] = G->adj[i]->next;
        }
    }
}