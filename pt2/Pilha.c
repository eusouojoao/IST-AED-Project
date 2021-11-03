#include <stdio.h>
#include <stdlib.h>
#include "Item.h"
#include "Common.h"
#include "Pilha.h"

static Item *gP = NULL; //stack em si
static int gN;          //assianala a primeira posição desocupada da pilha

/**
 * @brief  Função que inicializa a pilha
 * @note   
 * @param  pSize: dimensão maxima da pilha
 * @retval None
 */
void initPilha(unsigned pSize)
{
    gP = (Item *)malloc(pSize * sizeof(Item));
    if (gP == NULL)
    {
        exit(0);
    }
    gN = 0;
}

/**
 * @brief  Coloca um elemento na pilha
 * @note   
 * @param  pI: Item a colocar na pilha
 * @retval None
 */
void push(Item pI)
{
    gP[gN++] = pI;
}

/**
 * @brief  Função que verifica se a pilha está vazia
 * @note   
 * @retval 1 se a pilha estiver vazia
 *         0 se a pilha não estiver vazia
 */
int isEmpty()
{
    if (gN == 0)
        return 1;
    return 0;
}

/**
 * @brief  Função que retira um elemento da pilha
 * @note   
 * @retval Item retirado da pilha
 */
Item pop()
{
    if (!isEmpty())
    {
        Item aux = gP[--gN];
        return aux;
    }
    exit(0);
}

/**
 * @brief  Função que desaloca memória da pilha
 * @note   
 * @retval None
 */
void freePilha()
{
    if (gP != NULL)
        free(gP);
    return;
}

/**
 * @brief  Verifica se 2 peças estão na mesma sala com auxilio da pilha
 * @note   
 * @param  *board: tabuleiro (vetor unidimensional)
 * @param  n_Lines: número total de linhas do tabuleiro 
 * @param  n_Col: número total de colunas do tabuleiro
 * @param  p1: peça a verificar se está na mesma sala de p2
 * @param  p2: peça a verificar se está na mesma sala de p1
 * @retval 0 se as peças não estiverem na mesma sala
 *         1 se as peças estiverem na mesma sala
 */
int checkSameRoom(int *board, int n_Lines, int n_Col, int p1, int p2)
{
    int aux = p1;
    if (board[p1] != 0 || board[p2] != 0) // pelo menos uma das peças não é branca
        return 0;
    initPilha(4 * n_Lines + 4 * n_Col); //inicializa a pilha
    push(aux);
    while (!isEmpty())
    {
        aux = pop();
        if (aux == p2)
        {
            freePilha();
            return 1; //encontrou a peça 2 na mesma sala
        }
        pushAdjacent(board, aux, n_Lines, n_Col);
    }
    freePilha();
    return 0; //não encontrou a peça 2 na mesma sala
}
