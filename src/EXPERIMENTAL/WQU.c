#include <stdio.h>
#include <stdlib.h>
#include "item.h"
#include "common.h"

/**
 * @brief  Algoritmo Weighted Quick Union, faz a conexão entre 2 números p e q
 * @note   
 * @param  *id: vetor com as "etiquetas" dos diferentes números
 * @param  *sz: vetor com o "tamanho" das diferentes arvores associadas aos números
 * @param  p: número associado ao tabuleiro
 * @param  q: número associado ao tabuleiro
 * @retval None 
 */
void WQU(int *id, int *sz, int p, int q)
{
    int i, j;
    for (i = p; i != id[i]; i = id[i]) //procura
        ;
    for (j = q; j != id[j]; j = id[j])
        ;
    if (i == j)
        return;
    if (sz[i] < sz[j]) //une a árvore mais pequena à maior
    {
        id[i] = j; //união
        sz[j] += sz[i];
    }
    else
    {
        id[j] = i;
        sz[i] += sz[j];
    }
}

/**
 * @brief  Função que "pede" para fazer a união entre os numeros adjacentes a
 *         "tile" que pertençam à mesma sala (ou seja board[aux] == 0)
 * @note   
 * @param  *id: vetor com as "etiquetas" dos diferentes números
 * @param  *sz: vetor com o "tamanho" das diferentes arvores associadas aos números
 * @param  *board: tabuleiro (vetor unidimensional)
 * @param  tile: peça à qual se vão verificar as adjacencias e fazer as uniões
 * @param  n_Lines: número total de linhas do tabuleiro 
 * @param  n_Col: número total de colunas do tabuleiro
 * @retval None
 */
void connectivityAdjacentWQU(int *id, int *sz, int *board, int tile, int n_Lines, int n_Col)
{
    int aux = tile;
    aux++;                                              //verificar a peça adjacente à direita
    if (adjacentTileP(aux, n_Lines, n_Col, 1) == 1) //1-soma horizontal
    {
        if (board[aux] == 0)
        {
            WQU(id, sz, tile, aux); //união entre tile e aux
        }
    }
    aux = aux - 2;                                      //verificar a peça adjacente à esquerda
    if (adjacentTileP(aux, n_Lines, n_Col, 2) == 1) //2-subtração horizontal
    {
        if (board[aux] == 0)
        {
            WQU(id, sz, tile, aux);
        }
    }
    aux++;
    aux = aux + n_Col;                                  //verificar a peça adjacente em cima
    if (adjacentTileP(aux, n_Lines, n_Col, 0) == 1) //0-soma vertical
    {
        if (board[aux] == 0)
        {
            WQU(id, sz, tile, aux);
        }
    }
    aux = aux - 2 * n_Col;                              //verificar a peça adjacente em baixo
    if (adjacentTileP(aux, n_Lines, n_Col, 0) == 1) //0-subtração vertical
    {
        if (board[aux] == 0)
        {
            WQU(id, sz, tile, aux);
        }
    }
}

/**
 * @brief  Verifica se 2 peças estão na mesma sala com base n o algoritmo de WeightedQuickUnion
 * @note   Esta função também vai escrever -2 nas posições do tabuleiro que une
 * @param  *board: tabuleiro (vetor unidimensional)
 * @param  n_Lines: número total de linhas do tabuleiro 
 * @param  n_Col: número total de colunas do tabuleiro
 * @param  p1: peça a verificar se está na mesma sala de p2
 * @param  p2: peça a verificar se está na mesma sala de p1
 * @retval 0 se as peças não estiverem na mesma sala
 *         1 se as peças estiverem na mesma sala
 */
int sameRoomWQU(int *board, int n_Lines, int n_Col, int p1, int p2)
{
    int i, j, N = n_Col * n_Lines, aux = 0;
    int *id = (int *)malloc(N * sizeof(int)); //aloca vetor das "etiquetas"
    if (id == NULL)
        exit(0);

    int *sz = (int *)malloc(N * sizeof(int)); //aloca vetor de "altura" das árvores
    if (id == NULL)
        exit(0);
    for (i = 0; i < N; i++) //inicializa vetor das "etiquetas"
    {
        id[i] = i;
        sz[i] = 1;
    }
    if (p2 < p1) //coloca p1 como peça de numero menor
    {
        i = p2;
        p2 = p1;
        p1 = i;
    }
    for (i = p1; i < N; i++)
    {
        if (board[i] == 0)
        {
            board[i] = -2;                                             //alterar o valor do tabuleiro para informar que a peça já foi "unida"
            connectivityAdjacentWQU(id, sz, board, i, n_Lines, n_Col); //verifica conetividade com as peças adjacentes
        }
    }
    for (i = p1; i != id[i]; i = id[i]) //faz a procura para p1
        ;
    for (j = p2; j != id[j]; j = id[j]) //faz a procura para p2
        ;
    if (j == i) //verifica se p1 e p2 estão na mesma sala
    {
        aux = 1;
        free(id);
        free(sz);
        return aux; //retorna 1 caso p1 e p2 estejam na mesma sala
    }
    for (i = 0; i < p1; i++)
    {
        if (board[i] == 0)
        {
            board[i] = -2;
            connectivityAdjacentWQU(id, sz, board, i, n_Lines, n_Col);
        }
    }
    for (i = p1; i != id[i]; i = id[i])
        ;
    for (j = p2; j != id[j]; j = id[j])
        ;
    if (j == i)
        aux = 1;
    free(id);
    free(sz);
    return aux;
}
