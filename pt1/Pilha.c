#include <stdio.h>
#include <stdlib.h>
#include "Item.h"
#include "Common.h"
#include "Pilha.h"

static Item *gP = NULL;
static int gN;

void initPilha(unsigned pSize)
{
    gP = (Item *)malloc(pSize * sizeof(Item));
    if (gP == NULL)
    {
        exit(0);
    }
    gN = 0;
}

void push(Item pI)
{
    gP[gN++] = pI;
}

int isEmpty()
{
    if (gN == 0)
        return 1;
    return 0;
}

Item pop()
{
    if (!isEmpty())
    {
        Item aux = gP[--gN];
        return aux;
    }
    exit(0);
}

void freePilha()
{
    if (gP != NULL)
        free(gP);
    return;
}

int checkSameRoom(int *board, int n_Lines, int n_Col, int p1, int p2)
{
    int aux = p1;

    initPilha(n_Lines * n_Col);
    if (board[aux] == 0)
        push(aux);
    else {
        freePilha();
        return 0;
    }
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
