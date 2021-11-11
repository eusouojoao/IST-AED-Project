#include <stdio.h>
#include <stdlib.h>
#include "readInputFile.h"
#include "common.h"
#include "pilha.h"
#include "WQU.h"
#include "writeOutputFile.h"
#include "modosA.h"

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

/* caso seja o modo A6 na primeira parte do projeto comporta-se como (l2, c2) */
struct A6
{
    int l2, c2;
};

/* regras do tabuleiro, necessárias para o criar */
struct _boardRules
{
    struct board board;
    struct key key;
    struct A6 A6;
    int n_walls;
    char gameMode[3];
};

/* parede (l1, c1) com valor weight */
typedef struct _wall
{
    int l1, c1, weight;
} wall;

/**
 * @brief  Verifica a variante do jogo (A1..A6)
 * @note   
 *
 * @param  *var: variante do jogo em modo string
 * @retval Retorna a variante do jogo 1..6 
 */
int checkVariante(char *var)
{
    if (var[2] == '\0' && var[0] == 'A' && (var[1] > '0' && var[1] < '7'))
        return (var[1] - '0');
    return 0;
}

/**
/**
 * @brief   Inicia o modo de jogo consoante a variante especificada no ficheiro de input
 * @note    Tendo em conta o modo de jogo alguns dos parametros podem estar a
 *          NULL, mas como é obvio não serão utilizados 
 * @param  *brp: apontador para as regras do tabuleiro 
 * @param  *board: tabuleiro
 * @param  *output: nome do ficheiro de output
 * @param  A1: peso da parede no modo A1
 * @param  *AA: vector com as 4 possíveis peças adjacentes
 * @param  **A5: vetor bidimensional com a linha, a coluna e o custo de todas as peças adjacentes
 * @param  n_adj: numero de peças adjacentes à peça a ser testada
 * @param  tesouro: posição do "tesouro" no tabuleiro
 * @retval None
 */
void initGameMode(boardRules *brp, int *board, char *output, int A1, int *AA, int **A5, int n_adj, int tesouro)
{
    int A = -3; /* inicializar com valor fora da gama (erro) */

    if (brp->gameMode[1] == '1')
    {
        if ((checkInsideBoard(brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column)) == 0)
        {
            A = -2;
        }
        else
            A = A1;

        write2outputFile(output, A);
    }
    else if ((brp->gameMode[1] == '2') || (brp->gameMode[1] == '3') || (brp->gameMode[1] == '4'))
    {
        if ((checkInsideBoard(brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column)) == 0)
        {
            A = -2;
        }
        else
            A = checkAA(AA, (int)(brp->gameMode[1] - '0'), n_adj);

        write2outputFile(output, A);
    }
    else if (brp->gameMode[1] == '5')
    {
        if ((checkInsideBoard(brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column)) == 0)
        {
            A = -2;
        }
        else
            A = checkA5(A5, n_adj, brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column, tesouro);

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
            /* quando memória utilizada pela conectividade é inferior a cerca de 84MB */
            if (brp->board.lines * brp->board.columns < 2000 * 3500)
            {
                //conetividade WQU
                A = sameRoomWQU(board, brp->board.lines, brp->board.columns, convertTile(brp->key.Line, brp->key.Column, brp->board.columns), convertTile(brp->A6.l2, brp->A6.c2, brp->board.columns));
            }
            else
            {
                //utilizar pilha (), pois ocupa menos memória
                A = checkA6(board, brp->board.lines, brp->board.columns, convertTile(brp->key.Line, brp->key.Column, brp->board.columns), convertTile(brp->A6.l2, brp->A6.c2, brp->board.columns));
            }
        }

        write2outputFile(output, A);
    }

    return;
}

/**
 * @brief  Função que lê do ficheiro de input
 * @note   Preenche o brp (apontador para boardRules) com as regras do jogo,
 *         i.e., tamanho tabuleiro, variante do jogo... e inicializa o tabuleiro ao ler
 *         do ficheiro
 * @param  *fp: file pointer 
 * @param  *brp: apontador para as regras do tabuleiro
 * @param  *output: nome do ficheiro de output 
 * @retval None
 */
void readInputFile(FILE *fp, boardRules *brp, char *output)
{
    //---------------------------//
    int *board = NULL, *A = NULL, **A5 = NULL;
    int i = 0, j = 0, k = 0, tesouro = 0;
    int n_adj = 4, A1 = -1337;

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
    if ((fscanf(fp, "%d %d %d %d %2s", &(brp->board.lines), &(brp->board.columns),
                &(brp->key.Line), &(brp->key.Column), brp->gameMode)) != 5)
    {
        free(brp);
        free(Wall);
        return;
    }

    //---------------------------//
    if ((i = checkVariante(brp->gameMode)) == 6)
    {
        /* segunda coordenada (l2, c2), sendo que (l1, c2) são brp->key.Line e brp->key.Column */
        if (fscanf(fp, "%d %d", &(brp->A6.l2), &(brp->A6.c2)) != 2)
            exit(0);
    }

    //---------------------------//
    /* obtém o nº de paredes */
    if (fscanf(fp, "%d", &(brp->n_walls)) != 1)
        exit(0);

    //---------------------------//
    /* inicializa o «tabuleiro» com o tamanho especificado/necessário */
    if (i == 1)
        A1 = 0;
    else if (i >= 2 && i <= 4)
    {
        /* verificar se tile é uma peça lateral */
        if ((brp->key.Line == brp->board.lines) ||
            (brp->key.Column == brp->board.columns) ||
            (brp->key.Line == 1) || (brp->key.Column == 1))
            n_adj = 3;

        /* verificar se é uma peça de canto */
        else if ((brp->key.Line == 1 && brp->key.Column == 1) ||
                 (brp->key.Line == 1 && brp->key.Column == brp->board.columns) ||
                 (brp->key.Line == brp->board.lines && brp->key.Column == 1) ||
                 (brp->key.Line == brp->board.lines && brp->key.Column == brp->board.columns))
            n_adj = 2;

        A = (int *)malloc(n_adj * sizeof(int));
        if (A == NULL)
            exit(0);

        for (k = 0; k < n_adj; k++)
            A[k] = 0;
    }
    else if (i == 5)
    {
        /* verificar se tile é uma peça lateral */
        if ((brp->key.Line == brp->board.lines) ||
            (brp->key.Column == brp->board.columns) ||
            (brp->key.Line == 1) || (brp->key.Column == 1))
            n_adj = 3;

        /* verificar se é uma peça de canto */
        else if ((brp->key.Line == 1 && brp->key.Column == 1) ||
                 (brp->key.Line == 1 && brp->key.Column == brp->board.columns) ||
                 (brp->key.Line == brp->board.lines && brp->key.Column == 1) ||
                 (brp->key.Line == brp->board.lines && brp->key.Column == brp->board.columns))
            n_adj = 2;

        A5 = (int **)malloc(n_adj * sizeof(int *));
        if (A5 == NULL)
            exit(0);

        for (k = 0; k < n_adj; k++)
        {
            A5[k] = (int *)malloc(3 * sizeof(int));
            if (A5[k] == NULL)
                exit(0);
        }

        for (k = 0; k < n_adj; k++)
            for (j = 0; j < 2; j++)
                A5[k][j] = 0;
    }
    else
    {
        /* verifica se a alocação foi sucedida */
        if ((board = (int *)malloc(brp->board.lines * brp->board.columns * sizeof(int))) == NULL)
        {
            exit(0);
        }
        inicializeBoard(board, brp->board.lines, brp->board.columns);
    }

    //---------------------------//
    /* ler paredes do ficheiro de input */
    if (i == 1)
    {
        /* procurar a peça especifica, se for parede */
        for (/* stares into the void */; brp->n_walls > 0; brp->n_walls--)
        {

            if (fscanf(fp, "%d %d %d", &(Wall->l1), &(Wall->c1), &(Wall->weight)) != 3)
                exit(0);

            if ((Wall->l1 == brp->key.Line) && (Wall->c1 == brp->key.Column))
                A1 = Wall->weight;
        }
    }
    else if (i >= 2 && i <= 4)
    {
        /* apenas procurar pelas adjacentes */
        for (j = 0; brp->n_walls > 0; brp->n_walls--)
        {

            if (fscanf(fp, "%d %d %d", &(Wall->l1), &(Wall->c1), &(Wall->weight)) != 3)
                exit(0);

            if (adjacentTileLC(brp->key.Line, brp->key.Column, Wall->l1, Wall->c1) == 1)
            {
                A[j] = Wall->weight;
                j++;
            }
        }
    }
    else if (i == 5)
    {
        for (j = 0; brp->n_walls > 0; brp->n_walls--)
        {
            /* procurar as paredes adjacentes para verificar se é quebravel */
            if (fscanf(fp, "%d %d %d", &(Wall->l1), &(Wall->c1), &(Wall->weight)) != 3)
                exit(0);

            if (Wall->l1 == brp->key.Line && Wall->c1 == brp->key.Column)
                tesouro = Wall->weight;

            /* necessária informação das coordenadas das paredes para saber a posição relativa */
            if (adjacentTileLC(brp->key.Line, brp->key.Column, Wall->l1, Wall->c1) == 1)
            {
                A5[j][0] = Wall->l1;
                A5[j][1] = Wall->c1;
                A5[j][2] = Wall->weight;
                j++;
            }
        }
    }
    else
    {

        for (/* stares into the void */; brp->n_walls > 0; brp->n_walls--)
        {

            if (fscanf(fp, "%d %d %d", &(Wall->l1), &(Wall->c1), &(Wall->weight)) != 3)
                exit(0);

            /* preencher o tabuleiro com as paredes */
            board[convertTile(Wall->l1, Wall->c1, brp->board.columns)] = Wall->weight;
        }
    }

    //---------------------------//
    /* inicializa o modo de jogo */
    initGameMode(brp, board, output, A1, A, A5, n_adj, tesouro);

    if (i == 6)
        free(board);
    if (i == 5)
    {
        for (j = n_adj - 1; j >= 0; j--)
            free(A5[j]);
        free(A5);
    }
    if (i >= 2 && i <= 4)
        free(A);

    free(Wall);
    free(brp);

    //---------------------------//
    /* verifica se há outro tabuleiro, se sim, chama readInputFile() recursivamente */
    if (fp != NULL)
    {
        readInputFile(fp, brp, output);
    }

    return;
}
