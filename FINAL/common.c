#include "common.h"

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
 *         através da função convertTile() 
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
            board[convertTile(i, j, n_Col)] = 0; //inicializar a 0
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
 * @brief  Verifica se após uma soma/subtração horizontal ou vertical (adicionar
 *         1 ou n_Col), a peça resultante faz parte do tabuleiro e é adjacente
 * @note   Nesta função pressupõe-se que a peça, cuja a operação após a qual
 *         obteu-se a "tile", pertence ao tabuleiro 
 *          A função verifica a adjacencia de uma peça tendo em conta as coordenadas
 *          convertidas.
 * @param  tileP: Peça a verificar adjacencia
 * @param  n_Lines: número total de linhas do tabuleiro 
 * @param  n_Col: número total de colunas do tabuleiro
 * @param  HouV: Informa se operação realizada (anteriormente) à tile foi uma
 *         soma horizontal (HouV=1) ou subtração horizontal (HouV=2) 
 * @retval 0 se a peça estiver fora do tabuleiro, ou não fôr adjacente
 *         1 se a peça for adjacente e pertencer ao tabuleiro
 */
int adjacentTileP(int tileP, int n_Lines, int n_Col, int HouV)
{
    if (tileP < 0 || tileP > (n_Lines * n_Col - 1)) //fora das dimensões do tabuleiro
        return 0;                                   //não é adjacente
    if (HouV == 1)                                  //HouV==1 soma horizontal
    {
        if ((tileP % n_Col) == 0) //após a soma horizontal a peça "saltou" para a outra ponta do tabuleiro
            return 0;             //não é adjacente
    }
    if (HouV == 2) //HouV==2 subtração horizontal
    {
        if ((tileP % n_Col) == (n_Col - 1)) //após a subtração horizontal a peça "saltou" para a outra ponta do tabuleiro
            return 0;                       //não é adjacente
    }
    return 1; //é adjacente
}
