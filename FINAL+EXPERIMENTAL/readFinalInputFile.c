#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "readFinalInputFile.h"
#include "common.h"
#include "grafo.h"
#include "algoritmo.h"
#include "divideRooms.h"
#include "assembleGraph.h"
#include "regular.h"
#include "experimental.h"

#define FINAL
#include "writeOutputFile.h"

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

/**
 * @brief   Função que indica se estamos perante um caso especifico
 * @note
 * @param  *brp: boardRules pointer, contem toda a informação relevante sobre as regras do tabuleiro
 * @retval  1 se for caso especifico, 0 caso contrário
 * */
bool casosEspecificos(boardRules *brp)
{
    if (brp->key.Line == 1 && brp->key.Column == 1)
        return 1;
    else if (brp->n_walls == 0)
        return 1;

    return 0;
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
    bool valido = 1, especifico = 0, expFlag = 0;
    int *board = NULL, *wallVec = NULL;
    //float percentagem = 0.0;

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

    //---------------------------//
    //percentagem = ( ((float)getWalls(brp)) / (float)(getBoardColumns(brp) * getBoardLines(brp)) ) * 100;
    //percentagem = 100.0;
    if ( (getWalls(brp) > 100000) )
    {
        /* alocação da memória necessária */
        allocs(fp, brp, &wallVec, &board, &valido, &especifico);

        //---------------------------//
        if (valido && !especifico && board[convertTile(brp->key.Line, brp->key.Column, brp->board.columns)] != 0)
        {
            /* garbage collector */
            free(wallVec);
            free(board);
            valido = 0;
        }

        //---------------------------//
        /* inicializa o jogo */
        expFlag = 0;
        init(brp, board, wallVec, valido, especifico, first, output, expFlag);
    }
    else
    {
        expFlag = 1;
        experimental(fp, brp, &valido, &especifico, first, output, expFlag);
    }

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

/**
 * @brief Função que retorna o número de paredes do tabuleiro
 * @param *brp: apontador para boardRules
 * @retval número de paredes do tabuleiro
 * */
int getWalls(boardRules *brp)
{
    return( brp->n_walls );
}

/**
 * @brief Função que retorna as linhas do tabuleiro
 * @param *brp: apontador para boardRules
 * @retval linhas do tabuleiro
 * */
int getBoardLines(boardRules *brp)
{
    return( brp->board.lines );
}

/**
 * @brief Função que retorna as colunas do tabuleiro
 * @param *brp: apontador para boardRules
 * @retval colunas do tabuleiro
 * */
int getBoardColumns(boardRules *brp)
{
    return( brp->board.columns );
}

/**
 * @brief Função que retorna a linha do tesouro
 * @param *brp: apontador para boardRules
 * @retval linha do tesouro
 * */
int getKeyLine(boardRules *brp)
{
    return( brp->key.Line );
}

/**
 * @brief Função que retorna a coluna do tesouro
 * @param *brp: apontador para boardRules
 * @retval coluna do tesouro
 * */
int getKeyColumn (boardRules *brp)
{
    return( brp->key.Column );
}
