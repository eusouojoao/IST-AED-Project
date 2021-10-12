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
 * @brief  Função de teste que imprime o tabuleiro APAGAR DEPOIS
 * @note   Tabuleiro é um vetor unidimensional cujo elemento (l,c) é acedido
 *         através board[l * n_Lines + c - 1]
 *         Imprime apenas as peças não brancas
 * @param  *board: tabuleiro (vetor unidimensional)
 * @param  n_Lines: número total de linhas do tabuleiro 
 * @param  n_Col: número total de colunas do tabuleiro
 * @retval None
 */
void printBoard(int *board, int n_Lines, int n_Col)
{
    int i, j;
    for (i = 1; i <= n_Lines; i++)
        for (j = 1; j <= n_Col; j++)
            if (board[(i - 1) * n_Col + j - 1] != 0) //imprime apenas as peças não brancas
                printf("Na posição (%d,%d) está uma peça de custo %d \n", i, j, board[(i - 1) * n_Col + j - 1]);
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
int checkInsideBoard(int *board, int n_Lines, int n_Col, int l, int c)
{
    if ((l >= n_Lines) || (l < 1) || (c >= n_Col) || (c < 1))
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
    if (checkInsideBoard(board, n_Lines, n_Col, l, c) == 0)
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
    int up, down, right, left;
    int auxC = c, auxL = l;
    if (checkInsideBoard(board, n_Lines, n_Col, l, c) == 0)
        return -2; //peça fora do tabuleiro
    if (checkPeca(board, n_Lines, n_Col, l, c) < 1)
        return -1; //peça não é cinzenta
    auxC++;        //peça da direita (right)
    if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
        right = board[(auxL - 1) * n_Col + auxC - 1];
    auxC = auxC - 2; //peça da esquerda (left)
    if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
        left = board[(auxL - 1) * n_Col + auxC - 1];
    auxC++;
    auxL++; //peça de cima (up)
    if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
        up = board[(auxL - 1) * n_Col + auxC - 1];
    auxL = auxL - 2; //peça de baixo (down)
    if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
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
    if (checkInsideBoard(board, n_Lines, n_Col, l, c) == 0) //peça fora de tabuleiro
        return -2;                                          //fora do tabuleiro
    if (mode == 2)                                          //procurar peça adjacente branca
    {
        auxC++; //verificar a peça adjacente à direita
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] == 0)
                return 1; //encontramos peça branca
        auxC = auxC - 2;  //verificar a peça adjacente à esquerda
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] == 0)
                return 1;
        auxC++;
        auxL++; //verificar a peça adjacente de cima
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] == 0)
                return 1;
        auxL = auxL - 2; //verificar a peça adjacente de baixo
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] == 0)
                return 1;
        return 0;
    }
    if (mode == 3) //procurar peça adjacente cinzenta
    {
        auxC++; //verificar a peça adjacente à direita
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] > 0)
                return 1; //encontramos peça cinzenta
        auxC = auxC - 2;  //verificar a peça adjacente à esquerda
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] > 0)
                return 1;
        auxC++;
        auxL++; //verificar a peça adjacente de cima
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] > 0)
                return 1;
        auxL = auxL - 2; //verificar a peça adjacente de baixo
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] > 0)
                return 1;
        return 0;
    }
    if (mode == 4) //procurar peça adjacente preta
    {
        auxC++; //verificar a peça adjacente à direita
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] == -1)
                return 1; //encontramos peça preta
        auxC = auxC - 2;  //verificar a peça adjacente à esquerda
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] == -1)
                return 1;
        auxC++;
        auxL++; //verificar a peça adjacente de cima
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[(auxL - 1) * n_Col + auxC - 1] == -1)
                return 1;
        auxL = auxL - 2; //verificar a peça adjacente de baixo
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
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
    push(aux);
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

void WQU(int *id, int *sz, int p, int q)
{
    int i, j, t, x;
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
        t = j;
    }
    else
    {
        id[j] = i;
        sz[i] += sz[j];
        t = i;
    }
    /*   for (i = p; i != id[i]; i = x)
    {
        x = id[i];
        id[i] = t;
    }
    for (j = p; j != id[j]; j = x)
    {
        x = id[j];
        id[j] = t;
    } parte da compressão de caminho*/
}

void conectivityAdjacent(int *id, int *sz, int *board, int tile, int n_Lines, int n_Col)
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

int sameRoom(int *board, int n_Lines, int n_Col, int p1, int p2)
{
    int i, j, p, q, t, x, N = n_Col * n_Lines, aux = 0;
    int *id = (int *)malloc(N * sizeof(int));
    int *sz = (int *)malloc(N * sizeof(int));
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
            conectivityAdjacent(id, sz, board, i, n_Lines, n_Col);
        }
    }
    for (i = 0; i < p1; i++)
    {
        if (board[i] == 0)
        {
            board[i] = -2;
            conectivityAdjacent(id, sz, board, i, n_Lines, n_Col);
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

/**
 * @brief  Faz a leitura dos parametros no ficheiro lido VER ISTO!!
 * @note   TEMOS DE SEPARAR EM 2 FUNÇÕES UMA QUE FAZ A LEITURA DOS PARAMETROS
 *         INICIAIS E OUTRA QUE FAZ A LEITURA DAS PAREDES EM SI
 * @param  *fp: apontador para o ficheiro lido
 * @retval None
 */
void leituraP(FILE *fp)
{
    int n_V = 0, n_Col = 0, n_Lines = 0, key_Line = -1, key_Col = -1, n_walls = 0, l = 0, c = 0, w = 0;
    char variante[2]; //vai guardar o modo de jogo (A1 ou A2 ou ...)
    fscanf(fp, "%d %d", &n_Lines, &n_Col);
    fscanf(fp, "%d %d", &key_Line, &key_Col);
    fscanf(fp, "%s", variante);
    n_V = n_Col * n_Lines;
    printf("numero de vertices é %d\n", n_V);
    printf("o tesouro está em %d %d\n", key_Line, key_Col);
    if ((strcmp(variante, "A1") == 0) || (strcmp(variante, "A2") == 0) || (strcmp(variante, "A3") == 0) || (strcmp(variante, "A4") == 0) || (strcmp(variante, "A5") == 0) || (strcmp(variante, "A6") == 0))
    {
        printf("a variante é %s\n", variante);
    }
    else
    {                                                                     //caso não haja variante
        rewind(fp);                                                       //voltar ao inicio do ficheiro
        fscanf(fp, "%d %d %d %d", &n_Lines, &n_Col, &key_Line, &key_Col); // volta a ler o q já tinha sido lido até à variante
    }
    fscanf(fp, "%d", &n_walls);
    printf("numero total de paredes é %d\n", n_walls);

    int *board = (int *)malloc(n_Col * n_Lines * sizeof(int)); //aloca tabuleiro
    if (board == NULL)
    {
        exit(EXIT_FAILURE);
    }
    inicializeBoard(board, n_Lines, n_Col); //inicializa o tabuleiro
    while (fscanf(fp, "%d %d %d", &l, &c, &w) != EOF)
    {
        printf("parede em %d %d com custo %d\n", l, c, w);
        board[(l - 1) * n_Col + c - 1] = w; //colocar a parede no tabuleiro
    }
    printf("\n\n");
    printBoard(board, n_Lines, n_Col);               //imprimir o tabuleiro (APAGAR SERVE SÓ PARA CHECK)
    int A1 = checkPeca(board, n_Lines, n_Col, 4, 5); //TESTE do modo A1
    printf("\n %d", A1);
    int A2 = checkAdjacencia(board, n_Lines, n_Col, 3, 2, 4); //TESTE do modo A2, A3 e A4
    printf("\n %d", A2);
    int A5 = checkBreakable(board, n_Lines, n_Col, 3, 3); //TESTE do modo A5
    printf("\n %d", A5);
    //pilha
    //int A6 = checkSameRoom(board, n_Lines, n_Col, convertTile(1, 1, n_Col), convertTile(100000, 10000, n_Col));
    //conetividade
    int A6 = sameRoom(board, n_Lines, n_Col, convertTile(1, 1, n_Col), convertTile(100000, 10000, n_Col));
    printf("\n A6= %d", A6);
    printf("\n");
    free(board);
}
