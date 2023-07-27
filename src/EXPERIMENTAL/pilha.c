#include <stdio.h>
#include <stdlib.h>
#include "item.h"
#include "common.h"
#include "pilha.h"

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

