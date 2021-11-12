#include <stdlib.h>
#include "common.h"
#include "grafo.h"

/**
 * @brief  Função que insere uma aresta no grafo
 * @note   
 * @param  *myGraph: aponteiro para o grafo
 * @param  room1: numero da sala 1
 * @param  room2: numero da sala 2
 * @param  p_wall: posição da parede
 * @param  weight_wall: peso da parede
 * @retval None
 */
void insertInGraph(Graph *myGraph, int room1, int room2, int p_wall, int weight_wall)
{
    Edge *newE;
    /*conversão para um número positivo das salas (note-se que os numeros das salas começavam em -2 e iam decrescendo)*/
    room1 = -room1 - 2;
    room2 = -room2 - 2;
    newE = newEdge(room1, room2, p_wall, weight_wall); //cria uma aresta
    graphInsertE(myGraph, newE);                       //insere a aresta no grafo
}

/**
 * @brief  Função que vai preencher o grafo
 * @note   
 * @param  *myGraph: apontador para o grafo
 * @param  *board: tabuleiro (vetor unidimensional)x 
 * @param  *wallVec: vetor com a posição no tabuleiro de todas as paredes
 * @param  n_walls: número de paredes
 * @param  n_Lines: número total de linhas do tabuleiro 
 * @param  n_Col: número total de colunas do tabuleiro
 * @retval None
 */
void fillGraph(Graph *myGraph, int *board, int *wallVec, int n_walls, int n_Lines, int n_Col)
{
    int i, aux, right, left, up, down;
    for (i = 0; i < n_walls; i++)
    {
        aux = wallVec[i];
        right = -1, left = -1, up = -1, down = -1;
        aux++;                                          //verificar a peça adjacente à direita
        if (adjacentTileP(aux, n_Lines, n_Col, 1) == 1) //1-soma horizontal
        {
            right = board[aux]; //atribuir o número da sala à variável right
        }
        aux = aux - 2;                                  //verificar a peça adjacente à esquerda
        if (adjacentTileP(aux, n_Lines, n_Col, 2) == 1) //2-subtração horizontal
        {
            left = board[aux];
        }
        aux++;
        aux = aux + n_Col;                              //verificar a peça adjacente em cima
        if (adjacentTileP(aux, n_Lines, n_Col, 0) == 1) //0-soma vertical
        {
            up = board[aux];
        }
        aux = aux - 2 * n_Col;                          //verificar a peça adjacente em baixo
        if (adjacentTileP(aux, n_Lines, n_Col, 0) == 1) // 0-subtração vertical
        {
            down = board[aux];
        }
        if ((down < -1 && up < -1) && (down != up)) //caso a peça de cima e de baixo sejam salas diferentes
            if (board[wallVec[i]] != -1)
                insertInGraph(myGraph, down, up, wallVec[i], board[wallVec[i]]);
        if ((right < -1 && left < -1) && (right != left)) //caso a peça da direita e da esquerda sejam salas diferentes
        {
            if (board[wallVec[i]] != -1)
                insertInGraph(myGraph, right, left, wallVec[i], board[wallVec[i]]);
        }
    }
}
