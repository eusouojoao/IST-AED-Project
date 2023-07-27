#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "acervo.h"

/* acervo */
struct _acervo
{
    int *acervoArray;               /* vai conter os vértices                       */
    int max_size;                   /* tamanho máximo do acervo (nº de vértices)    */
    int current_size;               /* posição livre no acervo                      */
    int (*comparador)(int, int);    /* função de comparação (portabilidade)         */
};

/**
 * @brief   Inicializa o acervo com o tamanho máximo (nº de vértices do grafo),
 *          e com a função de comparação
 * @note   
 * @param  A: apontador para o acervo
 * @param  comparador: função de comparações
 * @retval None
 */
void acervoInit(acervo **A, int (*comparador)(int, int), int size)
{
    (*A) = (acervo *)malloc(sizeof(acervo));
    if ((*A) == NULL)
        exit(0);

    (*A)->acervoArray = (int *)malloc(size * sizeof(int));
    if ((*A)->acervoArray == NULL)
        exit(0);

    (*A)->max_size = (*A)->current_size = 0;
    (*A)->comparador = comparador;

    return;
}

/**
 * @brief  Insere o vértice na posição de maior prioridade no acervo e compara com os filhos
 * @note   Ordena de cima para baixo (os mais prioritários têm menor custo) 
 * @param  *A: apontador para o acervo
 * @param  cnt: número da posição atual no acervo
 * @retval None
 */
void acervoInserirTopo(acervo *A, int cnt)
{
    int temp;                   /* auxiliar para armazenar o valor do parente   */
    int child;                  /* armazena as coords dos filhos                */
    bool left = 0, right = 0;   /* flags filho à esquerda e filho à direita     */

    /* filho à esquerda */
    if ((2 * cnt + 1) < A->current_size)
        left = true;

    /* filho à direita */
    if ((2 * cnt + 2) < A->current_size)
        right = true;

    if (left && right)
    {
        /* verificar qual dos filhos é menor */
        if (A->comparador(A->acervoArray[2 * cnt + 1], A->acervoArray[2 * cnt + 2]) < 0)
        {
            child = 2 * cnt + 1;
        }
        else
        {
            child = 2 * cnt + 2;
        }
    }
    else if (left)
    {
        child = 2 * cnt + 1;
    }
    else if (right)
    {
        child = 2 * cnt + 2;
    }
    else
    {
        return;
    }

    /* se o filho for menor, trocar */
    if (A->comparador(A->acervoArray[child], A->acervoArray[cnt]) < 0)
    {
        temp = A->acervoArray[cnt];
        A->acervoArray[cnt] = A->acervoArray[child];
        A->acervoArray[child] = temp;
        acervoInserirTopo(A, child);
    }

    return;
}

/**
 * @brief Insere o vértice na posição de menor prioridade e recursivamente acha a sua posição
 *          no acervo
 * @note   Ordena de baixo para cima
 * @param  *A: apontador para o acervo
 * @param  cnt: posição atual no acervo
 * @retval None
 */
void acervoInserirFundo(acervo *A, int cnt)
{
    int parent = (int)floor((double)cnt / 2);
    int temp;

    if (cnt == 0)
        return;

    /* se o filho for menor que o pai, trocar */
    if (A->comparador(A->acervoArray[cnt], A->acervoArray[parent]) < 0)
    {
        temp = A->acervoArray[cnt];
        A->acervoArray[cnt] = A->acervoArray[parent];
        A->acervoArray[parent] = temp;
    }

    /* continuar a procura até ao topo */
    acervoInserirFundo(A, parent);
}

/**
 * @brief  Adiciona um vértice ao acervo
 * @note   Por baixo (ou seja, no indice de menor prioridade)
 * @param  *A: apontador para o acervo
 * @param  it: vértice a adicionar
 * @retval None
 */
void acervoAdd(acervo *A, int it)
{
    A->acervoArray[A->current_size] = it;
    acervoInserirFundo(A, A->current_size);
    A->current_size++;
    return;
}

/**
 * @brief  Retorna o elemento de maior prioridade do acervo
 * @note   
 * @param  *A: apontador para o acervo
 * @retval elemento de maior prioridade (int)
 */
int acervoGet(acervo *A)
{
    int min; /* elemento de maior prioridade (no projeto, é a dist/peso minim@) */

    if (A->current_size == 0)
    {
        /* isto nunca deve acontecer */
        exit(0);
    }

    min = A->acervoArray[0];

    /* insere o elemento de menor prioridade do acervo no topo */
    A->acervoArray[0] = A->acervoArray[A->current_size - 1];
    A->current_size--;

    if (A->current_size != 1)
    {
        /* encontra a posição do elemento que estava no fim do acervo,
         * pois passou para a posição de maior prioridade 
         * */
        acervoInserirTopo(A, 0);
    }

    return min;
}

/**
 * @brief  Verifica se o acervo está vazio
 * @note   
 * @param  *A: apontador para o acervo
 * @retval 1 se estiver, 0 se não estiver
 */
int acervoEmpty (acervo *A)
{
    if (A->current_size != 0)
        return 0;
    return 1;
}

/**
 * @brief  Liberta a memória alocada dinamicamente para o acervo
 * @note   
 * @param  *A: apontador para o acervo
 * @retval None
 */
void freeAcervo (acervo *A)
{
    free(A->acervoArray);
    free(A);
    return;
}
