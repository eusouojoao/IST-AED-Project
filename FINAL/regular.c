#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "common.h"
#include "grafo.h"
#include "algoritmo.h"
#include "divideRooms.h"
#include "assembleGraph.h"
#include "readFinalInputFile.h"

#define FINAL
#include "writeOutputFile.h"

/* parede (l1, c1) com valor weight */
typedef struct _wall
{
    int l1, c1, weight;
} wall;

/**
 * @brief  Inicializa a memória para a leitura do ficheiro
 * @note   
 * @param  *fp: apontador para o ficheiro 
 * @param  *brp: boardRules pointer, contem toda a informação relevante sobre as regras do tabuleiro
 * @param  **board: apontador para o tabuleiro unidimensional
 * @param  **wallVec: apontador para o vetor de paredes (armazena as coordenadas)
 * @param  **Wall: apontador para o apontador com a estrutura auxiliar na leitura
 * @param  *valido: apontador para a flag de validade do tabuleiro
 * @param  *especifico: apontador para a flag que indica se o tabuleiro é especifico (ver função casosEspecificos) 
 * @retval None
 */
void allocs(FILE *fp, boardRules *brp, int **wallVec,
            int **board, bool *valido, bool *especifico)
{
    wall *Wall = NULL;
    int counter = 0, j = 0;

    //---------------------------//
    if ((*valido) && !(*especifico))
    {
        if ((Wall = (wall *)malloc(sizeof(wall))) == NULL)
        {
            exit(0);
        }
        if (((*wallVec) = (int *)malloc(getWalls(brp) * sizeof(int))) == NULL)
        {
            exit(0);
        }
        inicializeWallVector((*wallVec), getWalls(brp));

        //---------------------------//
        /* verifica se a alocação foi sucedida */
        if (((*board) = (int *)malloc(getBoardLines(brp) * getBoardColumns(brp) * sizeof(int))) == NULL)
        {
            exit(0);
        }
        inicializeBoard((*board), getBoardLines(brp), getBoardColumns(brp));

        //---------------------------//
        /* ler paredes do ficheiro de input */
        counter = getWalls(brp);
        for (/* stares into the void */; counter > 0; counter--)
        {

            if (fscanf(fp, "%d %d %d", &((Wall)->l1), &((Wall)->c1), &((Wall)->weight)) != 3)
                exit(0);

            /* preencher o tabuleiro com as paredes */
            (*board)[convertTile((Wall)->l1, (Wall)->c1, getBoardColumns(brp))] = (Wall)->weight;
            (*wallVec)[j] = convertTile((Wall)->l1, (Wall)->c1, getBoardColumns(brp));
            j++;
        }
    }
    //---------------------------//
    /* caso não seja valido, ou seja um caso especifico,
      precisamos de ler apenas as paredes para que o file pointer
      avance
    */
    else
    {
        if ((Wall = (wall *)malloc(sizeof(wall))) == NULL)
        {
            exit(0);
        }
        for (int aux = getWalls(brp); aux > 0; aux--)
        {
            if (fscanf(fp, "%d %d %d", &((Wall)->l1), &((Wall)->c1), &((Wall)->weight)) != 3)
            {
                exit(0);
            }
            if (Wall->l1 == getKeyLine(brp) && Wall->c1 == getKeyColumn(brp))
            {
                (*valido) = 0;
                (*especifico) = 0;
            }
        }
    }

    //---------------------------//
    free(Wall);
    return;
}

/**
 * @brief  Função que chama os algoritmos, caso seja necessário continuar, para descobrir o caminho mais barato no 
 *          tabuleiro da sala inicial até à sala do tesouro
 * @note   
 * @param  *brp: boardRules pointer, contem toda a informação relevante sobre as regras do tabuleiro
 * @param  *board: tabuleiro unidimensional
 * @param  *wallVec: vetor de paredes (armazena as coordenadas)
 * @param  *Wall: apontador para estrutura auxiliar na leitura das paredes do ficheiro
 * @param  valido: flag de validade do tabuleiro
 * @param  especifico: flag que indica se o tabuleiro é especifico (ver função casosEspecificos) 
 * @param  first: flag que indica se é o primeiro tabuleiro a ser lido do ficheiro de entrada
 * @param  *output: string com o nome do ficheiro de saída
 * @retval None
 */
void init(boardRules *brp, int *board, int *wallVec, bool valido, bool especifico, 
        bool first, char *output)
{
    int tesouro, n_rooms = 0;
    if (valido && !especifico)
    {
        n_rooms = divideRooms(board, getBoardLines(brp), getBoardColumns(brp), getKeyLine(brp), getKeyColumn(brp));
        if (n_rooms == 1)
        {
            free(board);
            free(wallVec);
            writeZero(output, first);
        }
        else
        {
            Graph *G = graphInit(n_rooms);
            fillGraph(G, board, wallVec, getWalls(brp), getBoardLines(brp), getBoardColumns(brp));

            /* obtem a sala do tesouro */
            tesouro = -board[convertTile(getKeyLine(brp), getKeyColumn(brp), getBoardColumns(brp))] - 2;

            /* garbage collector */
            free(board);
            free(wallVec);

            //---------------------------//
            algoritmo(G);

            /* escreve para o ficheiro de saída */
            writeSolution(output, G, tesouro, getBoardColumns(brp), first);
            graphDestroy(G);
        }
    }
    else
    {
        if (especifico)
            writeZero(output, first);
        else
            writeInvalid(output, first);
    }

    return;
}
