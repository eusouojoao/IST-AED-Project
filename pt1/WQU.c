#include <stdio.h>
#include <stdlib.h>
#include "Item.h"
#include "Common.h"

void WQU(int *id, int *sz, int p, int q)
{
    int i, j;
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
    }
    else
    {
        id[j] = i;
        sz[i] += sz[j];
    }
}

void connectivityAdjacentWQU(int *id, int *sz, int *board, int tile, int n_Lines, int n_Col)
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

int sameRoomWQU(int *board, int n_Lines, int n_Col, int p1, int p2)
{
    int i, j, N = n_Col * n_Lines, aux = 0;
    int *id = (int *)malloc(N * sizeof(int));
    if (id == NULL) 
        exit(0);

    int *sz = (int *)malloc(N * sizeof(int));
    if (id == NULL) 
        exit(0);

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
            connectivityAdjacentWQU(id, sz, board, i, n_Lines, n_Col);
        }
    }
    for (i = p1; i != id[i]; i = id[i])
        ;
    for (j = p2; j != id[j]; j = id[j])
        ;
    if (j == i)
    {
        aux = 1;
        free(id);
        free(sz);
        return aux;
    }
    for (i = 0; i < p1; i++)
    {
        if (board[i] == 0)
        {
            board[i] = -2;
            connectivityAdjacentWQU(id, sz, board, i, n_Lines, n_Col);
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
