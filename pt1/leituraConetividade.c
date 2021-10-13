#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "Item.h"
#include "pilha.h"

/**
 * @brief  Converte coordenadas do tabuleiro, por exemplo (3,2) num único inteiro
 * @note   
 * @param  l: linha em que está a peça
 * @param  c: coluna em que está a peça
 * @param  n_Col: número total de colunas do tabuleiro
 * @retval Inteiro correspondente à conversão das coordenadas
 */
int convertTile(int l, int c, int n_Col)
{
    int aux = (l - 1) * n_Col + c - 1;
    return aux;
}

/**
 * @brief  Função que inicializa o tabuleiro a zeros
 * @note   Tabuleiro é um vetor unidimensional cujo elemento (l,c) é acedido
 *         através board[l * n_Lines + c - 1]
 * @param  *board: tabuleiro (vetor unidimensional)
 * @param  n_Lines: número total de linhas do tabuleiro 
 * @param  n_Col: número total de colunas do tabuleiro
 * @retval None
 */
void inicializeBoard(int *board, int n_Lines, int n_Col)
{
    int i, j;
    for (i = 1; i <= n_Lines; i++)
        for (j = 1; j <= n_Col; j++)
            board[(i - 1) * n_Col + j - 1] = 0; //inicializar a 0
}

/**
 * @brief  Verifica se uma peça está dentro ou fora do tabuleiro
 * @note   
 * @param  *board: tabuleiro (vetor unidimensional)
 * @param  n_Lines: número total de linhas do tabuleiro 
 * @param  n_Col: número total de colunas do tabuleiro
 * @param  l: linha em que está a peça
 * @param  c: coluna em que está a peça
 * @retval 0 se a peça estiver fora do tabuleiro
 *         1 se a peça estiver dentro do tabuleiro
 */
int checkInsideBoard(int n_Lines, int n_Col, int l, int c)
{
    if ((l > n_Lines) || (l < 1) || (c > n_Col) || (c < 1))
        return 0; //fora do tabuleiro
    return 1;     //dentro do tabuleiro
}

/**
 * @brief  Informa que tipo de peça é, e o seu custo
 * @note   Tabuleiro é um vetor unidimensional cujo elemento (l,c) é acedido
 *         através board[l * n_Lines + c - 1]  
 * @param  *board: tabuleiro (vetor unidimensional)
 * @param  n_Lines: número total de linhas do tabuleiro 
 * @param  n_Col: número total de colunas do tabuleiro
 * @param  l: linha em que está a peça
 * @param  c: coluna em que está a peça
 * @retval -2 se a peça estiver fora do tabuleiro
 *         o "peso" da peça caso esta esteja no tabuleiro
 */
int checkPeca(int *board, int n_Lines, int n_Col, int l, int c)
{
    if (checkInsideBoard(n_Lines, n_Col, l, c) == 0)
        return -2; //fora do tabuleiro
    return board[(l - 1) * n_Col + c - 1];
}

/**
 * @brief  Verifica se uma peça é cinzenta, e caso seja verifica se é quebrável ou não
 * @note   Tabuleiro é um vetor unidimensional cujo elemento (l,c) é acedido
 *         através board[l * n_Lines + c - 1]  
 * @param  *board: tabuleiro (vetor unidimensional)
 * @param  n_Lines: número total de linhas do tabuleiro 
 * @param  n_Col: número total de colunas do tabuleiro
 * @param  l: linha em que está a peça
 * @param  c: coluna em que está a peça
 * @retval -2 se a peça estiver fora do tabuleiro
 *         -1 se a peça não for cinzenta
 *         1 se a peça for cinzenta quebrável
 *         0 se a peça for cinzenta não quebrável
 */
int checkBreakable(int *board, int n_Lines, int n_Col, int l, int c)
{
    int up = -1, down = -1, right = -1, left = -1;
    int auxC = c, auxL = l;
    if (checkInsideBoard(n_Lines, n_Col, l, c) == 0)
        return -2; //peça fora do tabuleiro
    if (checkPeca(board, n_Lines, n_Col, l, c) < 1)
        return -1; //peça não é cinzenta
    auxC++;        //peça da direita (right)
    if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
        right = board[(auxL - 1) * n_Col + auxC - 1];
    auxC = auxC - 2; //peça da esquerda (left)
    if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
        left = board[(auxL - 1) * n_Col + auxC - 1];
    auxC++;
    auxL++; //peça de cima (up)
    if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
        up = board[(auxL - 1) * n_Col + auxC - 1];
    auxL = auxL - 2; //peça de baixo (down)
    if (checkInsideBoard( n_Lines, n_Col, auxL, auxC) == 1)
        down = board[(auxL - 1) * n_Col + auxC - 1];
    //peça da direita e a da esquerda brancas ou peça de cima e de baixo brancas => peça é quebrável
    if ((right == 0 && left == 0) || (up == 0 && down == 0))
        return 1; //peça cinzenta quebrável
    return 0;     //peça cinzenta não quebrável
}

/**
 * @brief  Verifica se uma peça tem alguma adjacente (branca, cinzenta ou preta)
 *         tem em conta se é o mode (2, 3 ou 4 respetivamente)
 * @note   Tabuleiro é um vetor unidimensional cujo elemento (l,c) é acedido
 *         através board[l * n_Lines + c - 1]   
 * @param  *board: tabuleiro (vetor unidimensional)
 * @param  n_Lines: número total de linhas do tabuleiro 
 * @param  n_Col: número total de colunas do tabuleiro
 * @param  l: linha em que está a peça
 * @param  c: coluna em que está a peça
 * @param  mode: define se procura-se uma peça adjacente branca, cinzenta ou
 *         preta (mode 2, 3 e 4 respetivamente)
 * @retval -2 se a peça estiver fora do tabuleiro
 *         0 se não houver peça adjacente da "cor" requerida
 *         1 se houver peça adjacente da "cor" requerida
 *         -5 erro
 */
int checkAdjacencia(int *board, int n_Lines, int n_Col, int l, int c, int mode)
{
    int auxC = c, auxL = l;
    if (checkInsideBoard(n_Lines, n_Col, l, c) == 0) //peça fora de tabuleiro
        return -2;                                          //fora do tabuleiro
    if (mode == 2)                                          //procurar peça adjacente branca
    {
        auxC++; //verificar a peça adjacente à direita
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] == 0)
                return 1; //encontramos peça branca
        auxC = auxC - 2;  //verificar a peça adjacente à esquerda
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] == 0)
                return 1;
        auxC++;
        auxL++; //verificar a peça adjacente de cima
        if (checkInsideBoard( n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] == 0)
                return 1;
        auxL = auxL - 2; //verificar a peça adjacente de baixo
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] == 0)
                return 1;
        return 0;
    }
    if (mode == 3) //procurar peça adjacente cinzenta
    {
        auxC++; //verificar a peça adjacente à direita
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] > 0)
                return 1; //encontramos peça cinzenta
        auxC = auxC - 2;  //verificar a peça adjacente à esquerda
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] > 0)
                return 1;
        auxC++;
        auxL++; //verificar a peça adjacente de cima
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] > 0)
                return 1;
        auxL = auxL - 2; //verificar a peça adjacente de baixo
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] > 0)
                return 1;
        return 0;
    }
    if (mode == 4) //procurar peça adjacente preta
    {
        auxC++; //verificar a peça adjacente à direita
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] == -1)
                return 1; //encontramos peça preta
        auxC = auxC - 2;  //verificar a peça adjacente à esquerda
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] == -1)
                return 1;
        auxC++;
        auxL++; //verificar a peça adjacente de cima
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] == -1)
                return 1;
        auxL = auxL - 2; //verificar a peça adjacente de baixo
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] == -1)
                return 1;
        return 0;
    }
    return -5; //erro
}

int checkInsideBoardP(int tile, int n_Lines, int n_Col, int HouV)
{
    if (tile < 0 || tile > (n_Lines * n_Col - 1))
        return 0;  //não é adjacente
    if (HouV == 1) //HouV==1 soma horizontal
    {
        if ((tile % n_Col) == 0)
            return 0; //não é adjacente
    }
    if (HouV == 2) //HouV==2 subtração horizontal
    {
        if ((tile % n_Col) == (n_Col - 1))
            return 0; //não é adjacente
    }
    return 1; //é adjacente
}

void pushAdjacent(int *board, int tile, int n_Lines, int n_Col)
{
    int aux = tile;
    aux++;
    if (checkInsideBoardP(aux, n_Lines, n_Col, 1) == 1) //soma horizontal
    {
        if (board[aux] == 0)
        {
            board[aux] = -2;
            push(aux);
        }
    }
    aux = aux - 2;
    if (checkInsideBoardP(aux, n_Lines, n_Col, 2) == 1) //subtração horizontal
    {
        if (board[aux] == 0)
        {
            board[aux] = -2;
            push(aux);
        }
    }
    aux++;
    aux = aux + n_Col;
    if (checkInsideBoardP(aux, n_Lines, n_Col, 0) == 1) //soma vertical
    {
        if (board[aux] == 0)
        {
            board[aux] = -2;
            push(aux);
        }
    }
    aux = aux - 2 * n_Col;
    if (checkInsideBoardP(aux, n_Lines, n_Col, 0) == 1) //subtração vertical
    {
        if (board[aux] == 0)
        {
            board[aux] = -2;
            push(aux);
        }
    }
}

int checkSameRoom(int *board, int n_Lines, int n_Col, int p1, int p2)
{
    int aux = p1;

    // initPilha(2 * n_Col + 3 * n_Lines); //ocupação máxima que a pilha poderia ter
    initPilha(n_Lines * n_Col);
    if (board[aux] == 0)
        push(aux);
    else
        return 0;
    while (!isEmpty())
    {
        aux = pop();
        if (aux == p2)
        {
            return 1; //encontrou a peça 2 na mesma sala
        }
        pushAdjacent(board, aux, n_Lines, n_Col);
    }
    return 0; //não encontrou a peça 2 na mesma sala
}

void QuickUnion(int *id, int p, int q)
{
    int i, j;
    for (i = p; i != id[i]; i = id[i])
        ;
    for (j = q; j != id[j]; j = id[j])
        ;
    if (i == j)
        return;
    else
    {
        id[j] = i;
    }
    /*for (i = p; i != id[i]; i = x)
    {
        x = id[i];
        id[i] = t;
    }
    for (j = p; j != id[j]; j = x)
    {
        x = id[j];
        id[j] = t;
    } //parte da compressão de caminho */
}

void WQU(int *id, int *sz, int p, int q)
{
    int i, j;
    for (i = p; i != id[i]; i = id[i])
        ;
    for (j = q; j != id[j]; j = id[j])
        ;
    if (i == j)
        return;
    if (sz[i] < sz[j])
    {
        id[i] = j;
        sz[j] += sz[i];
    }
    else
    {
        id[j] = i;
        sz[i] += sz[j];
    }
    /*for (i = p; i != id[i]; i = x)
    {
        x = id[i];
        id[i] = t;
    }
    for (j = p; j != id[j]; j = x)
    {
        x = id[j];
        id[j] = t;
    } //parte da compressão de caminho */
}

void conectivityAdjacentWQU(int *id, int *sz, int *board, int tile, int n_Lines, int n_Col)
{
    int aux = tile;
    aux++;
    if (checkInsideBoardP(aux, n_Lines, n_Col, 1) == 1) //soma horizontal
    {
        if (board[aux] == 0)
        {
            //board[aux] = -2;
            WQU(id, sz, tile, aux);
        }
    }
    aux = aux - 2;
    if (checkInsideBoardP(aux, n_Lines, n_Col, 2) == 1) //subtração horizontal
    {
        if (board[aux] == 0)
        {
            //board[aux] = -2;
            WQU(id, sz, tile, aux);
        }
    }
    aux++;
    aux = aux + n_Col;
    if (checkInsideBoardP(aux, n_Lines, n_Col, 0) == 1) //soma vertical
    {
        if (board[aux] == 0)
        {
            //board[aux] = -2;
            WQU(id, sz, tile, aux);
        }
    }
    aux = aux - 2 * n_Col;
    if (checkInsideBoardP(aux, n_Lines, n_Col, 0) == 1) //subtração vertical
    {
        if (board[aux] == 0)
        {
            //board[aux] = -2;
            WQU(id, sz, tile, aux);
        }
    }
}

void conectivityAdjacentQU(int *id, int *board, int tile, int n_Lines, int n_Col)
{
    int aux = tile;
    aux++;
    if (checkInsideBoardP(aux, n_Lines, n_Col, 1) == 1) //soma horizontal
    {
        if (board[aux] == 0)
        {
            //board[aux] = -2;
            QuickUnion(id, tile, aux);
        }
    }
    aux = aux - 2;
    if (checkInsideBoardP(aux, n_Lines, n_Col, 2) == 1) //subtração horizontal
    {
        if (board[aux] == 0)
        {
            //board[aux] = -2;
            QuickUnion(id, tile, aux);
        }
    }
    aux++;
    aux = aux + n_Col;
    if (checkInsideBoardP(aux, n_Lines, n_Col, 0) == 1) //soma vertical
    {
        if (board[aux] == 0)
        {
            //board[aux] = -2;
            QuickUnion(id, tile, aux);
        }
    }
    aux = aux - 2 * n_Col;
    if (checkInsideBoardP(aux, n_Lines, n_Col, 0) == 1) //subtração vertical
    {
        if (board[aux] == 0)
        {
            //board[aux] = -2;
            QuickUnion(id, tile, aux);
        }
    }
}

int sameRoomQU(int *board, int n_Lines, int n_Col, int p1, int p2)
{
    int i, j, N = n_Col * n_Lines, aux = 0;
    int *id = (int *)malloc(N * sizeof(int));
    if (id == NULL)
        exit(0);

    if (p2 < p1)
    {
        i = p2;
        p2 = p1;
        p1 = i;
    }
    for (i = 0; i < N; i++)
    {
        id[i] = i;
    }
    for (i = p1; i < N; i++)
    {
        if (board[i] == 0)
        {
            board[i] = -2;
            conectivityAdjacentQU(id, board, i, n_Lines, n_Col);
        }
    }
    for (i = p1; i != id[i]; i = id[i])
        ;
    for (j = p2; j != id[j]; j = id[j])
        ;
    if (j == i)
    {
        aux = 1;
        free(id);
        return aux;
    }
    for (i = 0; i < p1; i++)
    {
        if (board[i] == 0)
        {
            board[i] = -2;
            conectivityAdjacentQU(id, board, i, n_Lines, n_Col);
        }
    }
    for (i = p1; i != id[i]; i = id[i])
        ;
    for (j = p2; j != id[j]; j = id[j])
        ;
    if (j == i)
        aux = 1;
    free(id);
    return aux;
}

int sameRoomWQU(int *board, int n_Lines, int n_Col, int p1, int p2)
{
    int i, j, N = n_Col * n_Lines, aux = 0;
    int *id = (int *)malloc(N * sizeof(int));
    if (id == NULL) 
        exit(0);

    int *sz = (int *)malloc(N * sizeof(int));
    if (id == NULL) 
        exit(0);

    if (p2 < p1)
    {
        i = p2;
        p2 = p1;
        p1 = i;
    }
    for (i = 0; i < N; i++)
    {
        id[i] = i;
        sz[i] = 1;
    }
    for (i = p1; i < N; i++)
    {
        if (board[i] == 0)
        {
            board[i] = -2;
            conectivityAdjacentWQU(id, sz, board, i, n_Lines, n_Col);
        }
    }
    for (i = p1; i != id[i]; i = id[i])
        ;
    for (j = p2; j != id[j]; j = id[j])
        ;
    if (j == i)
    {
        aux = 1;
        free(id);
        free(sz);
        return aux;
    }
    for (i = 0; i < p1; i++)
    {
        if (board[i] == 0)
        {
            board[i] = -2;
            conectivityAdjacentWQU(id, sz, board, i, n_Lines, n_Col);
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
