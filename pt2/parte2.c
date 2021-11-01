#include <stdlib.h>
#include "Common.h"
#include "Pilha.h"
#include "graph2.h"

void inicializeWallVector(int *walls, int n_walls)
{
    for (int i = 0; i < n_walls; i++)
    {
        walls[i] = 0;
    }
}

void pushAdjacentTiles(int *board, int *walls, int tile, int n_Lines, int n_Col, int room)
{
    int aux = tile, i;
    aux++;
    if (isTileAdjacent(aux, n_Lines, n_Col, 1) == 1) //soma horizontal
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
    if (isTileAdjacent(aux, n_Lines, n_Col, 2) == 1) //soma horizontal
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
    if (isTileAdjacent(aux, n_Lines, n_Col, 0) == 1) //soma horizontal
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
    if (isTileAdjacent(aux, n_Lines, n_Col, 0) == 1) //soma horizontal
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
    int aux = 0, cursor = 0, room = 1;
    initPilha(2 * n_Col + 3 * n_Lines); //ocupação máxima que a pilha poderia ter
    while (isTileAdjacent(cursor, n_Lines, n_Col, 0) == 1)
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
            if (board[cursor] == 0 || isTileAdjacent(cursor, n_Lines, n_Col, 0) == 0)
                break;
    }
    freePilha();
    return --room;
}

void pushTiles(int *board, int tile, int n_Lines, int n_Col, int room)
{
    int aux = tile;
    aux++;
    if (isTileAdjacent(aux, n_Lines, n_Col, 1) == 1) //soma horizontal
    {
        if (board[aux] == 0)
        {
            board[aux] = -(room);
            push(aux);
        }
    }
    aux = aux - 2;
    if (isTileAdjacent(aux, n_Lines, n_Col, 2) == 1) //subtração horizontal
    {
        if (board[aux] == 0)
        {
            board[aux] = -(room);
            push(aux);
        }
    }
    aux++;
    aux = aux + n_Col;
    if (isTileAdjacent(aux, n_Lines, n_Col, 0) == 1) //soma vertical
    {
        if (board[aux] == 0)
        {
            board[aux] = -(room);
            push(aux);
        }
    }
    aux = aux - 2 * n_Col;
    if (isTileAdjacent(aux, n_Lines, n_Col, 0) == 1) //subtração vertical
    {
        if (board[aux] == 0)
        {
            board[aux] = -(room);
            push(aux);
        }
    }
}

int divideRooms(int *board, int n_Lines, int n_Col)
{
    int aux = 0, cursor = 0, room = 1;
    initPilha(n_Col * n_Lines); //ocupação máxima que a pilha poderia ter
    while (isTileAdjacent(cursor, n_Lines, n_Col, 0) == 1)
    {
        room++;
        aux = cursor;
        push(aux);
        while (!isEmpty())
        {
            aux = pop();
            pushTiles(board, aux, n_Lines, n_Col, room);
        }
        for (; cursor < n_Lines * n_Col; cursor++)
        {
            if ((board[cursor] == 0 && cursor != aux) || isTileAdjacent(cursor, n_Lines, n_Col, 0) == 0)
                break;
            if (board[cursor] == 0 && cursor == aux)
                board[cursor] = -(room);
        }
    }
    freePilha();
    room--;
    return room;
}

void insertInGraph(Graph *myGraph, int room1, int room2, int p_wall, int weight_wall)
{
    Edge *newE;
    room1 = -room1 - 2; //conversão para numero psoitivo das salas (note-se que os numeros das salas começavam em -2 e iam decrescendo)
    room2 = -room2 - 2;
    newE = newEdge(room1, room2, p_wall, weight_wall);
    graphInsertE(myGraph, newE);
    //free(newE);
}

void fillGraph(Graph *myGraph, int *board, int *walls, int n_walls, int n_Lines, int n_Col)
{
    int i, aux, right, left, up, down;
    for (i = 0; i < n_walls; i++)
    {
        aux = walls[i];
        right = -1, left = -1, up = -1, down = -1;
        aux++;                                           //verificar a peça adjacente à direita
        if (isTileAdjacent(aux, n_Lines, n_Col, 1) == 1) //1-soma horizontal
        {
            right = board[aux];
        }
        aux = aux - 2;                                   //verificar a peça adjacente à esquerda
        if (isTileAdjacent(aux, n_Lines, n_Col, 2) == 1) //2-subtração horizontal
        {
            left = board[aux];
        }
        aux++;
        aux = aux + n_Col;                               //verificar a peça adjacente em cima
        if (isTileAdjacent(aux, n_Lines, n_Col, 0) == 1) //0-soma vertical
        {
            up = board[aux];
        }
        aux = aux - 2 * n_Col;                           //verificar a peça adjacente em baixo
        if (isTileAdjacent(aux, n_Lines, n_Col, 0) == 1) // 0-subtração vertical
        {
            down = board[aux];
        }
        if ((down < -1 && up < -1) && (down != up))
            if (board[walls[i]] != -1)
                insertInGraph(myGraph, down, up, walls[i], board[walls[i]]);
        if ((right < -1 && left < -1) && (right != left))
        {
            if (board[walls[i]] != -1)
                insertInGraph(myGraph, right, left, walls[i], board[walls[i]]);
        }
    }
}
