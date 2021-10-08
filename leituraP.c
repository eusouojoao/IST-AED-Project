#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
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
    for (i = 0; i < n_Lines; i++)
        for (j = 0; j < n_Col; j++)
            board[i * n_Lines + j - 1] = 0; //inicializar a 0
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
    for (i = 0; i < n_Lines; i++)
        for (j = 0; j < n_Col; j++)
            if (board[i * n_Lines + j - 1] != 0) //imprime apenas as peças não brancas
                printf("Na posição (%d,%d) está uma peça de custo %d \n", i, j, board[i * n_Lines + j - 1]);
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
    if ((l > n_Lines) || (l < 0) || (c > n_Col) || (c < 0))
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
    return board[l * n_Lines + c - 1];
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
        right = board[auxL * n_Lines + auxC - 1];
    auxC = auxC - 2; //peça da esquerda (left)
    if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
        left = board[auxL * n_Lines + auxC - 1];
    auxC++;
    auxL++; //peça de cima (up)
    if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
        up = board[auxL * n_Lines + auxC - 1];
    auxL = auxL - 2; //peça de baixo (down)
    if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
        down = board[auxL * n_Lines + auxC - 1];
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
            if (board[auxL * n_Lines + auxC - 1] == 0)
                return 1; //encontramos peça branca
        auxC = auxC - 2;  //verificar a peça adjacente à esquerda
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[auxL * n_Lines + auxC - 1] == 0)
                return 1;
        auxC++;
        auxL++; //verificar a peça adjacente de cima
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[auxL * n_Lines + auxC - 1] == 0)
                return 1;
        auxL = auxL - 2; //verificar a peça adjacente de baixo
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[auxL * n_Lines + auxC - 1] == 0)
                return 1;
        return 0;
    }
    if (mode == 3) //procurar peça adjacente cinzenta
    {
        auxC++; //verificar a peça adjacente à direita
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[auxL * n_Lines + auxC - 1] > 0)
                return 1; //encontramos peça cinzenta
        auxC = auxC - 2;  //verificar a peça adjacente à esquerda
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[auxL * n_Lines + auxC - 1] > 0)
                return 1;
        auxC++;
        auxL++; //verificar a peça adjacente de cima
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[auxL * n_Lines + auxC - 1] > 0)
                return 1;
        auxL = auxL - 2; //verificar a peça adjacente de baixo
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[auxL * n_Lines + auxC - 1] > 0)
                return 1;
        return 0;
    }
    if (mode == 4) //procurar peça adjacente preta
    {
        auxC++; //verificar a peça adjacente à direita
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[auxL * n_Lines + auxC - 1] == -1)
                return 1; //encontramos peça preta
        auxC = auxC - 2;  //verificar a peça adjacente à esquerda
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[auxL * n_Lines + auxC - 1] == -1)
                return 1;
        auxC++;
        auxL++; //verificar a peça adjacente de cima
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[auxL * n_Lines + auxC - 1] == -1)
                return 1;
        auxL = auxL - 2; //verificar a peça adjacente de baixo
        if (checkInsideBoard(board, n_Lines, n_Col, auxL, auxC) == 1)
            if (board[auxL * n_Lines + auxC - 1] == -1)
                return 1;
        return 0;
    }
    return -5; //erro
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
        //fscanf(fp, "%*[^\n]\n*"); //saltar 2 linhas (MUDAR PARA FAZER 4 FSCANF %D )
        //fscanf(fp, "%*[^\n]\n*");
    }
    fscanf(fp, "%d", &n_walls);
    printf("numero total de paredes é %d\n", n_walls);

    int *board = (int *)malloc(n_Col * n_Lines * sizeof(int)); //aloca tabuleiro
    if (board == NULL)
    {
        exit(EXIT_FAILURE);
    }
    //int board[n_Lines][n_Col];
    inicializeBoard(board, n_Lines, n_Col); //inicializa o tabuleiro
    while (fscanf(fp, "%d %d %d", &l, &c, &w) != EOF)
    {
        printf("parede em %d %d com custo %d\n", l, c, w);
        board[l * n_Lines + c - 1] = w; //colocar a parede no tabuleiro
    }
    printf("\n\n");
    printBoard(board, n_Lines, n_Col);                //imprimir o tabuleiro (APAGAR SERVE SÓ PARA CHECK)
    int A1 = checkPeca(board, n_Lines, n_Col, 10, 9); //TESTE do modo A1
    printf("\n %d", A1);
    int A2 = checkAdjacencia(board, n_Lines, n_Col, 3, 2, 4); //TESTE do modo A2, A3 e A4
    printf("\n %d", A2);
    int A5 = checkBreakable(board, n_Lines, n_Col, 3, 3); //TESTE do modo A5
    printf("\n %d", A5);

    free(board);
}
