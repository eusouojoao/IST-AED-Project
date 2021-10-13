#include <stdio.h>
#include <stdlib.h>
#include "Item.h"
#include "pilha.h"

static Item *gP;
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
    free(gP);
}
