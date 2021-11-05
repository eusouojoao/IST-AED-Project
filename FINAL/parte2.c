#include <stdlib.h>
#include <stdio.h> //REMOVER
#include "Common.h"
#include "Pilha.h"
#include "grafo.h"

/**
 * @brief  Função que inicializa o vector de paredes
 * @note   
 * @param  *wallVec: vetor de paredes
 * @param  n_walls: número de paredes
 * @retval None
 */
void inicializeWallVector(int *wallVec, int n_walls)
{
    for (int i = 0; i < n_walls; i++)
        wallVec[i] = 0;
}

/**
 * @brief  Função que coloca que atribui uma sala às peças brancas adjacentes a "tile" e coloca-as na pilha
 * @note   
 * @param  *board: tabuleiro (vetor unidimensional)
 * @param  tile: peça à qual se vão verificar as adjacentes
 * @param  n_Lines: número total de linhas do tabuleiro 
 * @param  n_Col: número total de colunas do tabuleiro
 * @param  room: -número da sala a ser atribuido (as salas têm numeros negativos menor que 1)
 * @retval None
 */
void pushTiles(int *board, int tile, int n_Lines, int n_Col, int room)
{
    int aux = tile;
    aux++;                                           //verificar a peça adjacente à direita
    if (isTileAdjacent(aux, n_Lines, n_Col, 1) == 1) //1-soma horizontal
    {
        if (board[aux] == 0)
        {
            board[aux] = -(room);
            push(aux);
        }
    }
    aux = aux - 2;                                   //verificar a peça adjacente à esquerda
    if (isTileAdjacent(aux, n_Lines, n_Col, 2) == 1) //2-subtração horizontal
    {
        if (board[aux] == 0)
        {
            board[aux] = -(room);
            push(aux);
        }
    }
    aux++;
    aux = aux + n_Col;                               //verificar a peça adjacente em cima
    if (isTileAdjacent(aux, n_Lines, n_Col, 0) == 1) //0-soma vertical
    {
        if (board[aux] == 0)
        {
            board[aux] = -(room);
            push(aux);
        }
    }
    aux = aux - 2 * n_Col;                           //verificar a peça adjacente em baixo
    if (isTileAdjacent(aux, n_Lines, n_Col, 0) == 1) //0-subtração vertical
    {
        if (board[aux] == 0)
        {
            board[aux] = -(room);
            push(aux);
        }
    }
}

/**
 * @brief  Função cujo o papel é dividir o tabuleiro em salas
 * @note   após a função completar o seu trabalho, as paredes vao ficar com
 *         números >0, as paredes impassáveis com o números -1 e as salas com
 *         numeros <-1 (por exemplo a 1º sala com o número -2, a 2º sala com o
 *         numero -3 ...)
 * @param  *board: tabuleiro (vetor unidimensional)
 * @param  n_Lines: número total de linhas do tabuleiro 
 * @param  n_Col: número total de colunas do tabuleiro
 * @retval inteiro correspondente ao número de salas do tabuleiro
 */
int divideRooms(int *board, int n_Lines, int n_Col, int tesouroL, int tesouroC)
{
    int aux = 0, cursor = 0, room = 1;
    initPilha(n_Col * n_Lines);                            //ocupação máxima que a pilha poderia ter
    while (isTileAdjacent(cursor, n_Lines, n_Col, 0) == 1) //enquanto o cursor tiver dentro do tabuleiro
    {
        room++;
        aux = cursor;
        push(aux);
        while (!isEmpty())
        {
            aux = pop();
            pushTiles(board, aux, n_Lines, n_Col, room);
        }
        
        if (board[cursor] == 0 && cursor == aux)
                board[cursor] = -(room); //caso o cursor n chegue a avançar (sala com uma unica peça), é necessário atribuir um numero à sala

        if (board[0] == board[convertTile(tesouroL, tesouroC, n_Col)])
        {
            freePilha();
            return 1;
        }

        for (; cursor < n_Lines * n_Col; cursor++) //avançar o cursor até à proxima sala vazia
        {
            if ((board[cursor] == 0 && cursor != aux) || isTileAdjacent(cursor, n_Lines, n_Col, 0) == 0)
                break;
        }
    }
    freePilha();
    room--; // room foi inicializado a 1, logo o verdadeiro valor de room é menor por 1 unidade
    return room;
}

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
    room1 = -room1 - 2; //conversão para um número positivo das salas (note-se que os numeros das salas começavam em -2 e iam decrescendo)
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
        aux++;                                           //verificar a peça adjacente à direita
        if (isTileAdjacent(aux, n_Lines, n_Col, 1) == 1) //1-soma horizontal
        {
            right = board[aux]; //atribuir o número da sala à variável right
        }
        aux = aux - 2;                                   //verificar a peça adjacente à esquerda
        if (isTileAdjacent(aux, n_Lines, n_Col, 2) == 1) //2-subtração horizontal
        {
            left = board[aux];
        }
        aux++;
        aux = aux + n_Col;                               //verificar a peça adjacente em cima
        if (isTileAdjacent(aux, n_Lines, n_Col, 0) == 1) //0-soma vertical
        {
            up = board[aux];
        }
        aux = aux - 2 * n_Col;                           //verificar a peça adjacente em baixo
        if (isTileAdjacent(aux, n_Lines, n_Col, 0) == 1) // 0-subtração vertical
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
