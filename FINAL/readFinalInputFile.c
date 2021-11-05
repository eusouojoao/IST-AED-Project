#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "readFinalInputFile.h"
#include "Common.h"
#include "grafo.h"
#include "parte2.h"
#include "algoritmo.h"

#define FINAL
#include "writeOutputFile.h"

#define EXPERIMENTAL 1

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
 * @brief   Função que indica se estamos perante um caso especifico
 * @note
 * @param  *brp: boardRules pointer, contem toda a informação relevante sobre as regras do tabuleiro
 * @retval  1 se for caso especifico, 0 caso contrário
 * */
bool casosEspecificos (boardRules *brp)
{
    if ( brp->key.Line == 1 && brp->key.Column == 1 ) 
        return 1;
    else if ( brp->n_walls == 0 ) 
        return 1;

    return 0;
}

void allocs (FILE *fp, boardRules *brp, wall **Wall, int **wallVec,
             int **board, bool *valido, bool *especifico)
{
    int counter = 0, j = 0;

    //---------------------------//
    if ( (*valido) && !(*especifico) )
    {
        if ( ( (*Wall) = (wall *)malloc( sizeof(wall) ) ) == NULL)
        {
            exit(0);
        }
        if ( ( (*wallVec) = (int *)malloc( brp->n_walls * sizeof(int) ) ) == NULL )
        {
            exit(0);
        }
        inicializeWallVector( (*wallVec), brp->n_walls);

        //---------------------------//
        /* verifica se a alocação foi sucedida */
        if ( ((*board) = (int *)malloc(brp->board.lines * brp->board.columns * sizeof(int))) == NULL )
        {
            exit(0);
        }
        inicializeBoard( (*board), brp->board.lines, brp->board.columns);

        //---------------------------//
        /* ler paredes do ficheiro de input */
        counter = brp->n_walls;
        for (/* stares into the void */; counter > 0; counter--)
        {

            if (fscanf(fp, "%d %d %d", &( (*Wall)->l1), &( (*Wall)->c1), &( (*Wall)->weight)) != 3)
                exit(0);

            /* preencher o tabuleiro com as paredes */
            (*board)[convertTile( (*Wall)->l1, (*Wall)->c1, brp->board.columns)] = (*Wall)->weight;
            (*wallVec)[j] = convertTile( (*Wall)->l1, (*Wall)->c1, brp->board.columns);
            j++;
        }
    }
    //---------------------------//
    /* caso não seja valido, ou seja um caso especifico,
     * precisamos de ler apenas as paredes para que o file pointer
     * avance...
     * */
    else
    {
        if (( (*Wall) = (wall *)malloc(sizeof(wall))) == NULL)
        {
            exit(0);
        }
        for(/* D: */; brp->n_walls > 0 ; brp->n_walls--)
        {
            if (fscanf(fp, "%d %d %d", &( (*Wall)->l1), &( (*Wall)->c1), &( (*Wall)->weight)) != 3)
            {
                exit(0);
            }
            if ( (*Wall)->l1 == brp->key.Line && (*Wall)->c1 == brp->key.Column )
            {
                (*valido) = 0;
                (*especifico) = 0;
            }
        }
        free( (*Wall) );
    }

    //---------------------------//
    return;
}

void init (boardRules *brp, int *board, int *wallVec, wall *Wall,
            bool valido, bool especifico, bool first, char *output)
{
    int tesouro, n_rooms = 0;
    if (valido && !especifico)
    {
        n_rooms = divideRooms(board, brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column);
        if (n_rooms == 1)
        {
            free(board);
            free(Wall);
            free(wallVec);
            writeZero(output, first);    
        } 
        else
        {
            Graph *myGraph = graphInit(n_rooms);
            fillGraph(myGraph, board, wallVec, brp->n_walls, brp->board.lines, brp->board.columns);

            /* obtem a sala do tesouro */
            tesouro = ( -board[convertTile(brp->key.Line, brp->key.Column, brp->board.columns)] - 2 );

            /* garbage collector */
            free(board);
            free(Wall);
            free(wallVec);

            //---------------------------//
            algoritmo(myGraph);

            /* escreve para o ficheiro de saída */
            writeSolution(output, myGraph, tesouro, brp->board.columns, first);
            graphDestroy(myGraph);
        }
    }
    else
    {
        if (especifico)
            writeZero(output,first);
        else
            writeInvalid(output, first);
    }

    return;
}

/**
 * @brief  Função que lê o ficheiro de entrada e os multiplos tabuleiros dentro deste (se existirem) 
 * @note   Recursivamente lê todos os tabuleiros do ficheiro e cria os grafos especificos de cada
 * @param  *fp: apontador para o ficheiro de entrada
 * @param  *brp: boardRules pointer, contem toda a informação relevante sobre as regras do tabuleiro
 * @param  *output: nome do ficheiro de saída
 * @retval None
 */
void readFinalInputFile(FILE *fp, boardRules *brp, char *output)
{
    //---------------------------//
    static bool first = 1;
    bool valido = 1, especifico = 0;
    //wall *Wall = NULL;
    //int *board = NULL, *wallVec = NULL;

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

    especifico = casosEspecificos(brp);

#if !EXPERIMENTAL
    //---------------------------//
    /* alocação da memória necessária */
    allocs(fp, brp, &Wall, &wallVec, &board, &valido, &especifico); 

    //---------------------------//
    if (valido && !especifico && board[convertTile(brp->key.Line, brp->key.Column, brp->board.columns)] != 0)
    {
        /* garbage collector */
        free(Wall);
        free(wallVec);
        free(board);
        valido = 0;
    }

    //---------------------------//
    /* inicializa o jogo */
    init(brp, board, wallVec, Wall, valido, especifico, first, output);
#endif

    /*§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§*/
    /*                          EXPERIMENTAL                         */
    
    if (valido && !especifico)
    {

    }
    else
    {
        if (especifico)
            writeZero(output,first);
        else
            writeInvalid(output, first);
    }

    /*§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§*/

    first = 0; /* já não é o primeiro */

    //---------------------------//
    /* garbage collector */
    free(brp);

    //---------------------------//
    /* verifica se fp é válido, se sim, chama readInputFile() recursivamente */
    if (fp != NULL)
    {
        readFinalInputFile(fp, brp, output);
    }

    return;
}
