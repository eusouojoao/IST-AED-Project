#include "Pilha.h"

/**
 * @brief  Verifica se 2 peças são adjacentes
 * @note   
 * @param  l: linha da peça 1
 * @param  c: coluna da peça 1
 * @param  p2L: linha da peça 1
 * @param  p2C: coluna da peça 2
 * @retval 1 caso as 2 peças sejam adjacentes
 *         0 caso as 2 peças não sejam adjacentes
 */
int adjacentTile(int l, int c, int p2L, int p2C)
{
    if (((p2L == l + 1) || (p2L == l - 1)) && c == p2C) //adjacente vertical
        return 1;
    if (((p2C == c + 1) || (p2C == c - 1)) && l == p2L) //adjacente horizontal
        return 1;
    return 0;
}

/**
 * @brief  Verifica se há uma peça adjacente de uma determinada cor (depende do mode)
 * @note   
 * @param  *AA: vector com as 4 possíveis peças adjacentes
 * @param  mode: modo de jogo (A2, A3 ou A4) associado aos números (2, 3 e 4 respetivamente)
 * @retval 
 */
int checkAA(int *AA, int mode, int n_adj)
{
    int i;
    for (i = 0; i < n_adj; i++)
    {
        if (mode == 2)
            if (AA[i] == 0)
                return 1; //modo 2- branca adjacente
        if (mode == 3)
            if (AA[i] > 0)
                return 1; //modo 3- cinzenta adjacente
        if (mode == 4)
            if (AA[i] == -1)
                return 1; //modo 4- preta adjacente
    }
    return 0; //não tem a "cor" requerida adjacente
}

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
 * @brief  Informa que tipo de peça é, e o seu custo
 * @note   Tabuleiro é um vetor unidimensional cujo elemento (l,c) é acedido
 *         através da função convertTile() 
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
    return board[convertTile(l, c, n_Col)];
}

/**
 * @brief  Verifica se uma peça é cinzenta, e caso seja verifica se é quebrável ou não
 * @note   Tabuleiro é um vetor unidimensional cujo elemento (l,c) é acedido
 *         através da função convertTile() 
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
    int up = -2, down = -2, right = -2, left = -2; //inicializar as peças adjacentes como peças não brancas
    int auxC = c, auxL = l;
    if (checkInsideBoard(n_Lines, n_Col, l, c) == 0)
        return -2; //peça fora do tabuleiro
    if (checkPeca(board, n_Lines, n_Col, l, c) < 1)
        return -1; //peça não é cinzenta
    auxC++;        //peça da direita (right)
    if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
        right = board[convertTile(auxL, auxC, n_Col)];
    auxC = auxC - 2; //peça da esquerda (left)
    if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
        left = board[convertTile(auxL, auxC, n_Col)];
    auxC++;
    auxL++; //peça de cima (up)
    if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
        up = board[convertTile(auxL, auxC, n_Col)];
    auxL = auxL - 2; //peça de baixo (down)
    if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
        down = board[convertTile(auxL, auxC, n_Col)];
    //peça da direita e a da esquerda brancas ou peça de cima e de baixo brancas => peça é quebrável
    if ((right == 0 && left == 0) || (up == 0 && down == 0))
        return 1; //peça cinzenta quebrável
    return 0;     //peça cinzenta não quebrável
}

/**
 * @brief  Verifica se uma peça tem alguma adjacente (branca, cinzenta ou preta)
 *         tem em conta se é o mode (2, 3 ou 4 respetivamente)
 * @note   Tabuleiro é um vetor unidimensional cujo elemento (l,c) é acedido
 *         através da função convertTile() 
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
/*
int checkAdjacencia(int *board, int n_Lines, int n_Col, int l, int c, int mode)
{
    int auxC = c, auxL = l;
    if (checkInsideBoard(n_Lines, n_Col, l, c) == 0)
        return -2; //fora do tabuleiro
    if (mode == 2) //procurar peça adjacente branca
    {
        auxC++; //verificar a peça adjacente à direita
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[convertTile(auxL, auxC, n_Col)] == 0)
                return 1; //encontramos peça branca
        auxC = auxC - 2;  //verificar a peça adjacente à esquerda
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[convertTile(auxL, auxC, n_Col)] == 0)
                return 1;
        auxC++;
        auxL++; //verificar a peça adjacente de cima
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[convertTile(auxL, auxC, n_Col)] == 0)
                return 1;
        auxL = auxL - 2; //verificar a peça adjacente de baixo
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[convertTile(auxL, auxC, n_Col)] == 0)
                return 1;
        return 0;
    }
    if (mode == 3) //procurar peça adjacente cinzenta
    {
        auxC++; //verificar a peça adjacente à direita
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[convertTile(auxL, auxC, n_Col)] > 0)
                return 1; //encontramos peça cinzenta
        auxC = auxC - 2;  //verificar a peça adjacente à esquerda
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[convertTile(auxL, auxC, n_Col)] > 0)
                return 1;
        auxC++;
        auxL++; //verificar a peça adjacente de cima
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[convertTile(auxL, auxC, n_Col)] > 0)
                return 1;
        auxL = auxL - 2; //verificar a peça adjacente de baixo
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[convertTile(auxL, auxC, n_Col)] > 0)
                return 1;
        return 0;
    }
    if (mode == 4) //procurar peça adjacente preta
    {
        auxC++; //verificar a peça adjacente à direita
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[convertTile(auxL, auxC, n_Col)] == -1)
                return 1; //encontramos peça preta
        auxC = auxC - 2;  //verificar a peça adjacente à esquerda
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[convertTile(auxL, auxC, n_Col)] == -1)
                return 1;
        auxC++;
        auxL++; //verificar a peça adjacente de cima
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[convertTile(auxL, auxC, n_Col)] == -1)
                return 1;
        auxL = auxL - 2; //verificar a peça adjacente de baixo
        if (checkInsideBoard(n_Lines, n_Col, auxL, auxC) == 1)
            if (board[convertTile(auxL, auxC, n_Col)] == -1)
                return 1;
        return 0;
    }
    return -5; //erro
} */

/**
 * @brief  Verifica se após uma soma/subtração horizontal ou vertical (adicionar
 *         1 ou n_Col), a peça resultante faz parte do tabuleiro e é adjacente
 * @note   Nesta função pressupõe-se que a peça, cuja a operação após a qual
 *         obteu-se a "tile", pertence ao tabuleiro 
 * @param  tile: Peça a verificar adjacencia
 * @param  n_Lines: número total de linhas do tabuleiro 
 * @param  n_Col: número total de colunas do tabuleiro
 * @param  HouV: Informa se operação realizada (anteriormente) à tile foi uma
 *         soma horizontal (HouV=1) ou subtração horizontal (HouV=2) 
 * @retval 0 se a peça estiver fora do tabuleiro, ou não fôr adjacente
 *         1 se a peça for adjacente e pertencer ao tabuleiro
 */
int isTileAdjacent(int tile, int n_Lines, int n_Col, int HouV)
{
    if (tile < 0 || tile > (n_Lines * n_Col - 1)) //fora das dimensões do tabuleiro
        return 0;                                 //não é adjacente
    if (HouV == 1)                                //HouV==1 soma horizontal
    {
        if ((tile % n_Col) == 0) //após a soma horizontal a peça "saltou" para a outra ponta do tabuleiro
            return 0;            //não é adjacente
    }
    if (HouV == 2) //HouV==2 subtração horizontal
    {
        if ((tile % n_Col) == (n_Col - 1)) //após a subtração horizontal a peça "saltou" para a outra ponta do tabuleiro
            return 0;                      //não é adjacente
    }
    return 1; //é adjacente
}

/**
 * @brief  Função cujo o papel é fazer o push() (para a pilha), das peças adjacentes à tile
 * @note   Esta função também vai escrever -2 nas posições do tabuleiro a que se faz push()
 * @param  *board: tabuleiro (vetor unidimensional)
 * @param  tile: Peça à qual se vai fazer push() das adjacentes
 * @param  n_Lines: número total de linhas do tabuleiro 
 * @param  n_Col: número total de colunas do tabuleiro
 * @retval None
 */
void pushAdjacent(int *board, int tile, int n_Lines, int n_Col)
{
    int aux = tile;
    aux++;                                           //verificar a peça adjacente à direita
    if (isTileAdjacent(aux, n_Lines, n_Col, 1) == 1) //1-soma horizontal
    {
        if (board[aux] == 0) // caso a peça adjacente seja branca
        {
            board[aux] = -2; //alterar o valor do tabuleiro para informar que a peça já foi para a pilha
            push(aux);       //fazer o push para a pilha
        }
    }
    aux = aux - 2;                                   //verificar a peça adjacente à esquerda
    if (isTileAdjacent(aux, n_Lines, n_Col, 2) == 1) //2-subtração horizontal
    {
        if (board[aux] == 0)
        {
            board[aux] = -2;
            push(aux);
        }
    }
    aux++;
    aux = aux + n_Col;                               //verificar a peça adjacente em cima
    if (isTileAdjacent(aux, n_Lines, n_Col, 0) == 1) //0-soma vertical
    {
        if (board[aux] == 0)
        {
            board[aux] = -2;
            push(aux);
        }
    }
    aux = aux - 2 * n_Col;                           //verificar a peça adjacente em baixo
    if (isTileAdjacent(aux, n_Lines, n_Col, 0) == 1) // 0-subtração vertical
    {
        if (board[aux] == 0)
        {
            board[aux] = -2;
            push(aux);
        }
    }
}
