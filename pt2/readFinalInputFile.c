#include <stdio.h>
#include <stdlib.h>
#include "writeOutputFile.h"
#include "readFinalInputFile.h"
#include "Common.h"
#include "graph2.h"
#include "algoritmo.h"
#include "parte2.h"

#define tesouroSala ( -board[convertTile(brp->key.Line, brp->key.Column, brp->board.columns)] - 2 )

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

/**
 * @brief  
 * @note   
 * @param  *fp: 
 * @param  *brp: 
 * @param  *output: 
 * @retval None
 */
void readFinalInputFile(FILE *fp, boardRules *brp, char *output)
{
    //---------------------------//
    static bool first = 1;
    bool valido = 1;
    wall *Wall = NULL;
    int *board = NULL, *wallVec = NULL;
    int n_rooms = 0, j = 0, counter = 0;

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
        return;
    }

    if (!checkInsideBoard(brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column))
        valido = 0;

    //---------------------------//
    /* obtem o nº de paredes */
    if (fscanf(fp, "%d", &(brp->n_walls)) != 1)
        exit(0);

    //---------------------------//
    if (valido)
    {
        if ((Wall = (wall *)malloc(sizeof(wall))) == NULL)
        {
            exit(0);
        }
        if ((wallVec = (int *)malloc(brp->n_walls * sizeof(int))) == NULL)
        {
            exit(0);
        }
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
            board[convertTile(Wall->l1, Wall->c1, brp->board.columns)] = Wall->weight;
            wallVec[j] = convertTile(Wall->l1, Wall->c1, brp->board.columns);
            j++;
        }
    }
    else
    {
        for(/* D: */; brp->n_walls > 0 ; brp->n_walls--)
            if(fscanf(fp,"%*d %*d %*d") != 3)
                continue;
    }

    //---------------------------//
    if (valido && board[convertTile(brp->key.Line, brp->key.Column, brp->board.columns)] != 0)
    {
        free(Wall);
        free(wallVec);
        free(board);
        valido = 0;
    }

    //---------------------------//
    /* inicializa o jogo */
    if (valido)
    {
        n_rooms = divideRooms(board, brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column);
        if (n_rooms == 1)
        {
            writeZero(output, first);    
        } 
        else
        {
            Graph *myGraph = graphInit(n_rooms);
            fillGraph(myGraph, board, wallVec, brp->n_walls, brp->board.lines, brp->board.columns);
            algoritmo(myGraph);

            /* escreve para o ficheiro de saída */
            writeSolution(output, myGraph, tesouroSala, brp->board.columns, first);
            graphDestroy(myGraph);
        }
    }
    else
    {
        writeInvalid(output, first);
    }

    //---------------------------//
    first = 0;
    /* garbage collector */
    if (valido)
    {
        free(board);
        free(Wall);
        free(wallVec);
    }

    free(brp);

    //---------------------------//
    /* verifica se há outro tabuleiro, se sim, chama readInputFile() recursivamente */
    if (fp != NULL)
    {
        readFinalInputFile(fp, brp, output);
    }

    return;
}
