#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "Item.h"
#include "Pilha.h"
#include "graph2.h"
#include "parte2.h"

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

int checkIfAdjacent(int tile, int n_Lines, int n_Col, int HouV)
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
    if (checkIfAdjacent(aux, n_Lines, n_Col, 1) == 1) //soma horizontal
    {
        if (board[aux] == 0)
        {
            board[aux] = -2;
            push(aux);
        }
    }
    aux = aux - 2;
    if (checkIfAdjacent(aux, n_Lines, n_Col, 2) == 1) //subtração horizontal
    {
        if (board[aux] == 0)
        {
            board[aux] = -2;
            push(aux);
        }
    }
    aux++;
    aux = aux + n_Col;
    if (checkIfAdjacent(aux, n_Lines, n_Col, 0) == 1) //soma vertical
    {
        if (board[aux] == 0)
        {
            board[aux] = -2;
            push(aux);
        }
    }
    aux = aux - 2 * n_Col;
    if (checkIfAdjacent(aux, n_Lines, n_Col, 0) == 1) //subtração vertical
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
    initPilha(2 * n_Col + 3 * n_Lines); //ocupação máxima que a pilha poderia ter
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
    freePilha();
    return 0; //não encontrou a peça 2 na mesma sala
}

void inicializeWallVector(int *walls, int n_walls)
{
    int i, j;
    for (i = 0; i < 6 * n_walls; i++)
    {
        walls[i] = 0;
    }
}

void pushAdjacentTiles(int *board, int *walls, int tile, int n_Lines, int n_Col, int room)
{
    int aux = tile, i;
    aux++;
    if (checkIfAdjacent(aux, n_Lines, n_Col, 1) == 1) //soma horizontal
    {
        if (board[aux] == 0)
        {
            board[aux] = -(room);
            push(aux);
        }
        else if (board[aux] > 0)
        {
            for (i = 1; i < 5; i++)
            {
                if (walls[board[aux] + i] == room)
                    break;
                if (walls[board[aux] + i] == 0)
                {
                    walls[board[aux] + i] = room;
                    break;
                }
            }
            //        board[aux] = room;
        }
    }
    aux = aux - 2;
    if (checkIfAdjacent(aux, n_Lines, n_Col, 2) == 1) //soma horizontal
    {
        if (board[aux] == 0)
        {
            board[aux] = -(room);
            push(aux);
        }
        else if (board[aux] > 0)
        {
            for (i = 1; i < 5; i++)
            {
                if (walls[board[aux] + i] == room)
                    break;
                if (walls[board[aux] + i] == 0)
                {
                    walls[board[aux] + i] = room;
                    break;
                }
            }
            //        board[aux] = room;
        }
    }
    aux++;
    aux = aux + n_Col;
    if (checkIfAdjacent(aux, n_Lines, n_Col, 0) == 1) //soma horizontal
    {
        if (board[aux] == 0)
        {
            board[aux] = -(room);
            push(aux);
        }
        else if (board[aux] > 0)
        {
            for (i = 1; i < 5; i++)
            {
                if (walls[board[aux] + i] == room)
                    break;
                if (walls[board[aux] + i] == 0)
                {
                    walls[board[aux] + i] = room;
                    break;
                }
            }
            //        board[aux] = room;
        }
    }
    aux = aux - 2 * n_Col;
    if (checkIfAdjacent(aux, n_Lines, n_Col, 0) == 1) //soma horizontal
    {
        if (board[aux] == 0)
        {
            board[aux] = -(room);
            push(aux);
        }
        else if (board[aux] > 0)
        {
            for (i = 1; i < 5; i++)
            {
                if (walls[board[aux] + i] == room)
                    break;
                if (walls[board[aux] + i] == 0)
                {
                    walls[board[aux] + i] = room;
                    break;
                }
            }
            //        board[aux] = room;
        }
    }
}

int findRoom(int *board, int *walls, int n_Lines, int n_Col)
{
    int aux = 0, cursor = 0, room = 0;
    initPilha(2 * n_Col + 3 * n_Lines); //ocupação máxima que a pilha poderia ter
    while (checkIfAdjacent(cursor, n_Lines, n_Col, 0) == 1)
    {
        room++;
        aux = cursor;
        push(aux);
        while (!isEmpty())
        {
            aux = pop();
            pushAdjacentTiles(board, walls, aux, n_Lines, n_Col, room);
        }
        for (; cursor < n_Lines * n_Col; cursor++)
            if (board[cursor] == 0 || checkIfAdjacent(cursor, n_Lines, n_Col, 0) == 0)
                break;
    }
    freePilha();
    return room;
}

/*void printWalls(int *walls, int n_walls)
{
    int i;
    for (i = 0; i < 6 * n_walls; i = i + 6)
    {
        if ((walls[i + 2] != 0) && (walls[i + 3] != 0))
        {
            printf("Parede na posição %d entre a sala %d e %d\n", walls[i], walls[i + 2], walls[i + 3]);
        }
    }
} */

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
    int n_rooms, treasure, j;
    char variante[3]; //vai guardar o modo de jogo (A1 ou A2 ou ...)
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
    int *walls = (int *)malloc(n_walls * sizeof(int));
    if (walls == NULL)
    {
        exit(EXIT_FAILURE);
    }
    inicializeWallVector(walls, n_walls);
    j = 0;
    while (fscanf(fp, "%d %d %d", &l, &c, &w) != EOF)
    {
        printf("parede em %d %d com custo %d\n", l, c, w);
        board[(l - 1) * n_Col + c - 1] = w;
        walls[j] = (l - 1) * n_Col + c - 1;
        j++;
    }
    printf("\n\n");
    /* printBoard(board, n_Lines, n_Col);               //imprimir o tabuleiro (APAGAR SERVE SÓ PARA CHECK)
    int A1 = checkPeca(board, n_Lines, n_Col, 4, 5); //TESTE do modo A1
    printf("\n %d", A1);
    int A2 = checkAdjacencia(board, n_Lines, n_Col, 3, 2, 4); //TESTE do modo A2, A3 e A4
    printf("\n %d", A2);
    int A5 = checkBreakable(board, n_Lines, n_Col, 3, 3); //TESTE do modo A5
    printf("\n %d", A5);
    int A6 = checkSameRoom(board, n_Lines, n_Col, convertTile(5, 5, n_Col), convertTile(1, 1, n_Col));
    printf("\n %d", A6); */
    printf("\n");
    n_rooms = divideRooms(board, n_Lines, n_Col);
    printf("O total de salas é %d\n", n_rooms);
    treasure = board[convertTile(key_Line, key_Col, n_Col)];
    free(board);
    Graph *myGraph = graphInit(n_rooms);
    fillGraph(myGraph, board, walls, n_walls, n_Lines, n_Col);
    printWalls(walls, n_walls);
    graphDestroy(myGraph);
    free(walls);
}
