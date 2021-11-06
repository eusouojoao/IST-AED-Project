#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "acervo.h"
#include "grafo.h"
#include "algoritmo.h"

/* vetores distancia e parentes alocados globalmente porque são necessários 
  noutros ficheiros (para simplificar return values e passagens a funções)
  */
static int *dist;
static int *parent;

/**
 * @brief Compara as distancias da sala "a" e da sala "b" à sala inicial 
 * @note   
 * @param  a: sala "a"
 * @param  b: sala "b"
 * @retval -1 (menor), 0 (igual) ou 1 (maior)
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
 * @brief  Algoritmo que retorna os caminhos mais baratos face à sala inicial
 * @note   
 * @param  *G: grafo com todas as informações necessárias
 * @retval None
 */
void algoritmo(Graph *G)
{
    int v, w, Dt;
    void *t;
    int Vmax = getVertices(G);
    acervo *A = NULL;

    //---------------------------//
    dist = (int *)malloc(Vmax * sizeof(int));
    if (dist == NULL)
        exit(0);

    parent = (int *)malloc(Vmax * sizeof(int));
    if (parent == NULL)
        exit(0);

    //---------------------------//
    acervoInit(&A, comparaRooms, Vmax);

    //---------------------------//
    /* inicializar vetor de distancias e vetor das salas "parentes" */
    for (v = 0; v < Vmax; v++)
    {
        dist[v] = INT_MAX;
        parent[v] = -1;
    }

    //---------------------------//
    /* iniciar com a sala inicial */
    acervoAdd(A, 0);
    dist[0] = 0;

    //---------------------------//
    while (!acervoEmpty(A))
    {
        if (dist[(v = acervoGet(A))] != INT_MAX)
            for (assignLista(G, &t, v); t != NULL; listaNext(&t))
                if (dist[(w = getSala(t))] > (Dt = (dist[v] + getDist(t))))
                {
                    dist[w] = Dt;
                    acervoAdd(A, w);
                    parent[w] = v;
                }
    }

    //---------------------------//
    freeAcervo(A);
    return;
}

/**
 * @brief Retorna a linha da peça que foi transformada numa coordenada unidimensional 
 * @note   
 * @param  coord: coordenada unidimensional
 * @param  n_Col: número de colunas do tabuleiro
 * @retval linha (int)
 */
int Linha(int coord, int n_Col)
{
    return ((int)ceil((float)(coord + 1) / (float)n_Col));
}

/**
 * @brief  Retorna a coluna da peça que foi transformada numa coordenada unidimensional
 * @note   
 * @param  coord: coordenada unidimensional
 * @param  n_Lines: linha correspondente da peça
 * @param  n_Col: número de colunas do tabuleiro
 * @retval coluna (int)
 */
int Coluna(int coord, int n_Lines, int n_Col)
{
    return (coord + 1 - ((n_Lines - 1) * n_Col));
}

/**
 * @brief  Retorna a parede de menor custo ligada à sala atual
 * @note   Vai procurar na sala "parente" a parede (de menor custo btw) que a liga à sala atual 
 * @param  *list: lista de salas adjacentes à sala "parente"
 * @param  p: número da sala atual
 * @param  *wall: coordenada a parede
 * @param  *weight: peso da parede
 * @retval None
 */
void getParede(void *list, int p, int *wall, int *weight)
{
    void *aux = list;
    while (aux != NULL && getSala(aux) != p)
        listaNext(&aux);

    if (aux == NULL)
        exit(0);

    *wall = getWall(aux);
    *weight = getDist(aux);

    return;
}

/**
 * @brief  Imprime da sala inicial à sala do tesouro
 * @note   Como o vetor de salas "parentes" indica a sala anterior, é preciso percorrer até à sala
 *          inicial para imprimir ordenadamente as paredes de menor custo partidas 
 * @param  *fp: apontador para o ficheiro aberto, onde se vai escrever no modo "append"
 * @param  *G: grafo que contem toda a informação relativa ao problema
 * @param  p: número da sala atual
 * @param  n_Col: número de colunas do tabuleiro
 * @retval None
 */
void printRecursivo(FILE *fp, Graph *G, int p, int n_Col)
{
    int wall, weight;  /* parede em coordenadas unidimensionais e peso associado   */
    int linha, coluna; /* coordenadas bidimensionais da parede                     */
    void *t;           /* apontador para uma lista generica                        */

    //---------------------------//
    /* só para a chamada recursiva quando chegarmos à sala cujo parente é a sala inicial */
    if (parent[p] == 0)
    {

        assignLista(G, &t, 0);
        getParede(t, p, &wall, &weight);
        linha = Linha(wall, n_Col);
        coluna = Coluna(wall, linha, n_Col);
        fprintf(fp, "%d %d %d\n", linha, coluna, weight);

        return;
    }

    //---------------------------//
    /* continuar a aventura recursiva */
    printRecursivo(fp, G, parent[p], n_Col);

    //---------------------------//
    /* converter coordenada unidimensional em bidimensional, e obter o custo da parede */
    assignLista(G, &t, parent[p]);
    getParede(t, p, &wall, &weight);
    linha = Linha(wall, n_Col);
    coluna = Coluna(wall, linha, n_Col);

    //---------------------------//
    /* imprimir o resultado no formato especificado (l, c, w) */
    fprintf(fp, "%d %d %d\n", linha, coluna, weight);

    return;
}

/**
 * @brief  Imprime a solução do tabuleiro de entrada (um ficheiro pode conter vários!)
 * @note   É necessário incluir uma flag que indica se é a primeira solução a ser escrita
 *          no ficheiro
 * @param  *output: nome do ficheiro de output (ficheiro_de_entrada.sol)
 * @param  *G: grafo com toda a informação necessária sobre o tabuleiro
 * @param  tesouroRoom: sala em que o tesouro se encontra (ou seja, um vertice no grafo)
 * @param  n_Col: número de colunas do tabuleiro
 * @param  first: flag que indica se é o primeiro tabuleiro a ser escrito no ficheiro de
 *                  output
 * @retval None
 */
void writeSolution(char *output, Graph *G, int tesouroRoom, int n_Col, bool first)
{
    int i = tesouroRoom;    /* sala do tesouro                              */
    int distance = dist[i]; /* distancia da sala do tesouro à sala inicial  */
    int cnt = 0;            /* salas percorridas até à sala inicial         */

    //---------------------------//
    FILE *fp = fopen(output, "a+");
    if (fp == NULL)
        return;

    //---------------------------//
    /* se não for o primeiro tabuleiro a ser escrito no ficheiro é necessária uma gap */
    if (!first)
    {
        fprintf(fp, "\n\n");
    }

    //---------------------------//
    /* verificar se há caminho da sala do tesouro até à sala inicial */
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
        printRecursivo(fp, G, tesouroRoom, n_Col);
    }
    else
    {
        /* não existe caminho possivel */
        fprintf(fp, "-1\n");
    }

    //---------------------------//
    fclose(fp);
    free(dist);
    free(parent);
    return;
}
