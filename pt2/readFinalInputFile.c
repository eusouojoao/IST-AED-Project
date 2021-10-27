#include <stdio.h>
#include <stdlib.h>
#include "readFinalInputFile.h"
#include "Common.h"
#include "graph2.h"
#include "parte2.h"

/* linhas e colunas do tabuleiro */
struct board
{
    int lines, columns;
};

/* chave na segunda parte, na primeira é a peça (l1, c1) */
struct key
{
    int Line, Column;
};

/* regras do tabuleiro, necessárias para o criar */
struct _boardRules
{
    struct board board;
    struct key key;
    int n_walls;
};

/* parede (l1, c1) com valor weight */
typedef struct _wall
{
    int l1, c1, weight;
} wall;

/* APAGAR DEPOIS */

void printBoard(int *board, int columns, int lines)
{
    printf("lines:\t%d", lines);
    printf("columns:\t%d\n", columns);
    for (int j = 1; j <= lines; j++)
        for (int i = 1; i <= columns; i++)
        {
            if (i == (columns))
                printf("%d\t\n", board[((j - 1) * columns + i - 1)]);
            else
                printf("%d\t", board[((j - 1) * columns + i - 1)]);
        }

    return;
}

/* APAGAR DEPOIS */

/* adicionar comentarios lol */
/*
 * ¯\_(ツ)_/¯
 *
 * */
void readFinalInputFile(FILE *fp, boardRules *brp, char *output)
{
    //---------------------------//
    int *board = NULL, *wallVec = NULL;
    int n_rooms = 0, j = 0, counter = 0;

    //---------------------------//
    wall *Wall = (wall *)malloc(sizeof(wall));
    if (Wall == NULL)
        exit(0);

    //---------------------------//
    brp = (boardRules *)malloc(sizeof(boardRules));
    if (brp == NULL)
        exit(0);

    //---------------------------//
    /* obtem as dimensões do tabuleiro, as coordenadas da peça 1, e a variante do jogo (A1..6) */
    if ((fscanf(fp, "%d %d %d %d", &(brp->board.lines), &(brp->board.columns),
                &(brp->key.Line), &(brp->key.Column))) != 4)
    {
        free(brp);
        free(Wall);
        return;
    }
    //---------------------------//
    /* obtem o nº de paredes */
    if (fscanf(fp, "%d", &(brp->n_walls)) != 1)
        exit(0);

    wallVec = (int *)malloc(brp->n_walls * sizeof(int));
    if (wallVec == NULL)
        exit(0);

    inicializeWallVector(wallVec, brp->n_walls);

    //---------------------------//
    /* verifica se a alocação foi sucedida */
    if ((board = (int *)malloc(brp->board.lines * brp->board.columns * sizeof(int))) == NULL)
    {
        exit(0);
    }
    inicializeBoard(board, brp->board.lines, brp->board.columns);

    //---------------------------//
    /* ler paredes do ficheiro de input */
    counter = brp->n_walls;
    for (/* stares into the void */; counter > 0; counter--)
    {

        if (fscanf(fp, "%d %d %d", &(Wall->l1), &(Wall->c1), &(Wall->weight)) != 3)
            exit(0);

        /* preencher o tabuleiro com as paredes */
        board[(Wall->l1 - 1) * brp->board.columns + Wall->c1 - 1] = Wall->weight;
        wallVec[j] = (Wall->l1 - 1) * brp->board.columns + Wall->c1 - 1;
        j++;
    }

    //board[(brp->key.Line - 1) * brp->board.columns + brp->key.Column - 1] = -1337;
    /* testes (APAGAR depois) */
    printBoard(board, brp->board.columns, brp->board.lines);

    //---------------------------//
    /* inicializa o jogo */
    n_rooms = divideRooms(board, brp->board.lines, brp->board.columns);
    printf("\n\nO total de salas é %d\n\n\n", n_rooms);
    Graph *myGraph = graphInit(n_rooms);
    fillGraph(myGraph, board, wallVec, brp->n_walls, brp->board.lines, brp->board.columns);
    //teste
    printGraph(myGraph);
    /* garbage collector */
    free(board);
    free(Wall);
    free(wallVec);
    free(brp);
    graphDestroy(myGraph);

    //---------------------------//
    /* verifica se há outro tabuleiro, se sim, chama readInputFile() recursivamente */
    if (fp != NULL)
    {
        readFinalInputFile(fp, brp, output);
    }

    return;
}
