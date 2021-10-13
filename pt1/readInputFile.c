#include <stdio.h>
#include <stdlib.h>
#include "readInputFile.h"
#include "Common.h"
#include "writeOutputFile.h"
#include "Pilha.h"

struct board
{
    int lines, columns;
};

struct key
{
    int Line, Column;
};

struct A6
{
    int l2, c2;
};

struct _boardRules
{
    struct board board;
    struct key key;
    struct A6 A6;
    int n_walls;
    char gameMode[3];
};

typedef struct _wall
{
    int l1, c1, weight;
} wall;

/**
 * @brief  
 * @note   
 * @param  *var: 
 * @retval 
 */
int checkVariante(char *var)
{
    if (var[2] == '\0' && var[0] == 'A' && (var[1] > '0' && var[1] < '7'))
        return (var[1] - '0');
    return 0;
}

/*
 *  initGameMode()
 *
 *  Description: Inicia o modo de jogo consoante a variante especificada no ficheiro de input
 *
 *  Side-Effects: Inicia o jogo...
 *
 *  Returns: - (void)
 * */
/**
 * @brief  
 * @note   
 * @param  *brp: 
 * @param  *board: 
 * @param  *output: 
 * @retval None
 */
void initGameMode(boardRules *brp, int *board, char *output)
{
    /* !ATENÇÃO! apenas declaradas pra teste! */
    int A = -1337;

    if (brp->gameMode[1] == '1')
    {
        A = checkPeca(board, brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column);
        write2outputFile(output, A);
    }
    else if ((brp->gameMode[1] == '2') || (brp->gameMode[1] == '3') || (brp->gameMode[1] == '4'))
    {
        A = checkAdjacencia(board, brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column, (int)(brp->gameMode[1] - '0'));
        write2outputFile(output, A);
    }
    else if (brp->gameMode[1] == '5')
    {
        A = checkBreakable(board, brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column);
        write2outputFile(output, A);
    }
    else if (brp->gameMode[1] == '6')
    {
        if ((checkInsideBoard(brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column)) == 0 ||
            (checkInsideBoard(brp->board.lines, brp->board.columns, brp->A6.l2, brp->A6.c2)) == 0)
        {
            A = -2;
        }
        else
        {
            /* proceder... */
            //if (4 * 3 * brp->board.lines * brp->board.columns <= 90 * 1000000)
            //conetividade WQU
            //A = sameRoomWQU(board, brp->board.lines, brp->board.columns, convertTile(brp->key.Line, brp->key.Column, brp->board.columns), convertTile(brp->A6.l2, brp->A6.c2, brp->board.columns));
            //else if (4 * 2 * brp->board.lines * brp->board.columns > 90 * 1000000)
            //pilha
            A = checkSameRoom(board, brp->board.lines, brp->board.columns, convertTile(brp->key.Line, brp->key.Column, brp->board.columns), convertTile(brp->A6.l2, brp->A6.c2, brp->board.columns));
            //else
            //conetividade QU
            //A = sameRoomQU(board, brp->board.lines, brp->board.columns, convertTile(brp->key.Line, brp->key.Column, brp->board.columns), convertTile(brp->A6.l2, brp->A6.c2, brp->board.columns));
        }

        write2outputFile(output, A);
    }

    return;
}

/* ---
 *  readInputFile()
 *
 *  Description: Função que lê do ficheiro de input
 *
 *  Side-Effects: Preenche o brp (apontador para boardRules) com as regras do jogo, i.e.,
 *  tamanho tabuleiro, variante do jogo... e inicializa o tabuleiro ao ler do ficheiro
 *
 *  Returns: - (void)
 *
 * --- */
/**
 * @brief  
 * @note   
 * @param  *fp: 
 * @param  *brp: 
 * @param  *output: 
 * @retval None
 */
void readInputFile(FILE *fp, boardRules *brp, char *output)
{
    int *board = NULL, i = 0;
    char buf[128];
    wall *Wall = (wall *)malloc(sizeof(wall));
    if (Wall == NULL)
        exit(0);

    brp = (boardRules *)malloc(sizeof(boardRules));
    if (brp == NULL)
        exit(0);

    /* obtem as dimensões do tabuleiro, as coordenadas da peça 1, e a variante do jogo (A1..6) */
    if ((fscanf(fp, "%d %d %d %d %2s", &(brp->board.lines), &(brp->board.columns),
                &(brp->key.Line), &(brp->key.Column), brp->gameMode)) != 5)
    {
        free(brp);
        free(Wall);
        return;
    }

    /* isto é para a 2ª parte, possivelmente pode ser deleted */
    if ((i = checkVariante(brp->gameMode)) == 0)
    {
        rewind(fp);
        if (fscanf(fp, "%d %d %d %d", &(brp->board.lines), &(brp->board.columns),
                   &(brp->key.Column), &(brp->key.Line)) != 4)
            exit(0);
    }
    else if (i == 6)
    {
        /* segunda coordenada (l2, c2), sendo que (l1, c2) são brp->key.Line e brp->key.Column */
        if (fscanf(fp, "%d %d", &(brp->A6.l2), &(brp->A6.c2)) != 2)
            exit(0);
    }

    /* obtem o nº de paredes */
    if (fscanf(fp, "%d", &(brp->n_walls)) != 1)
        exit(0);

    /* verifica se a alocação foi sucedida */
    if ((board = (int *)malloc(brp->board.lines * brp->board.columns * sizeof(int))) == NULL)
    {
        exit(0);
    }

    /* inicializa o tabuleiro com o tamanho especificado */
    inicializeBoard(board, brp->board.lines, brp->board.columns);

    /* leitura das paredes */
    fseek(fp, -1L, SEEK_CUR);
    if (fscanf(fp, "%*[^\n]\n") != 0)
        exit(0);
    while (fgets(buf, 127, fp) != NULL)
    {
        if (buf[0] == '\n')
        {
            break; /* sai do while loop */
        }

        if (sscanf(buf, "%d %d %d", &(Wall->l1), &(Wall->c1), &(Wall->weight)) != 3)
            exit(0);

        /* !TESTE! REMOVER DEPOIS */
        board[(Wall->l1 - 1) * brp->board.columns + Wall->c1 - 1] = Wall->weight;
    }

    /* inicia o jogo */
    initGameMode(brp, board, output);
    free(Wall);
    free(board);
    free(brp);

    /* verifica se há outro tabuleiro, se sim, chama readInputFile() recursivamente */
    if (buf[0] == '\n')
    {
        readInputFile(fp, brp, output);
    }

    return;
}
