#include <stdio.h>
#include <stdlib.h>
#include "readInputFile.h"
#include "board.h"

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
void initGameMode (boardRules *brp, int *board)
{
    /* !ATENÇÃO! apenas declaradas pra teste! */
    int A = -1337;

    if (brp->gameMode[1] == '1')
    {
        A = checkPeca(board, brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column);
        printf ("A1:\t%d\n\n", A);
        /* substituir printf por função que escreve output file */

    } else if ((brp->gameMode[1] == '2') || (brp->gameMode[1] == '3') || (brp->gameMode[1] == '4')) {
        A = checkAdjacencia(board, brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column, (int) (brp->gameMode[1] - '0'));
        printf ("A%d:\t%d\n\n", (int) (brp->gameMode[1] - '0'), A);
        /* substituir printf por função que escreve output file */

    } else if (brp->gameMode[1] == '5') {
        A = checkBreakable(board, brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column);
        printf ("A5:\t%d\n\n", A);
        /* substituir printf por função que escreve output file */

    } else if (brp->gameMode[1] == '6') {
        if ((checkInsideBoard(board, brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column)) == 0 ||\
            (checkInsideBoard(board, brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Column)) == 0)
        {
            A = -2;
        } else {
            /* proceder... */
            ;
        }

        printf ("A6:\t%d\n\n", A);
        /* substituir printf por função que escreve output file */
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
void readInputFile (FILE *fp, boardRules *brp)
{
    int *board = NULL, i = 0;
    char buf[128];
    wall *Wall = (wall *)malloc(sizeof(wall));
    brp = (boardRules *)malloc(sizeof(boardRules));

    /* obtem as dimensões do tabuleiro, as coordenadas da peça 1, e a variante do jogo (A1..6) */
    fscanf(fp, "%d %d %d %d %2s", &(brp->board.lines), &(brp->board.columns), 
            &(brp->key.Line), &(brp->key.Column), brp->gameMode);
    
    printf("%d %d %d %d %s\n", brp->board.lines, brp->board.columns, brp->key.Line, brp->key.Line, brp->gameMode);

    /* isto é para a 2ª parte, possivelmente pode ser deleted */
    if ((i = checkVariante(brp->gameMode)) == 0)
    {
        rewind(fp);
        fscanf(fp, "%d %d %d %d", &(brp->board.lines), &(brp->board.columns), 
                &(brp->key.Column), &(brp->key.Line));
    
    } else if (i == 6) {
        /* segunda coordenada (l2, c2), sendo que (l1, c2) são brp->key.Line e brp->key.Column */
        fscanf(fp, "%d %d", &(brp->A6.l2), &(brp->A6.c2));
    }

    /* obtem o nº de paredes */
    fscanf(fp, "%d", &(brp->n_walls));

    /* verifica se a alocação foi sucedida */
    if ( (board = (int *)malloc( brp->board.lines * brp->board.columns * sizeof(int) )) == NULL )
    {
        exit(EXIT_FAILURE);
    }

    /* inicializa o tabuleiro com o tamanho especificado */
    inicializeBoard (board, brp->board.lines, brp->board.columns);

    /* leitura das paredes */
    fseek(fp, -1L, SEEK_CUR);
    fscanf(fp, "%*[^\n]\n");
    while (fgets(buf, 127, fp) != NULL )
    {
        if (buf[0] == '\n')
        {
            /* linha vazia, ou seja pode vir um novo tabuleiro a seguir */
            /* pensar no que fazer */
            /* possivelmente break e depois no final (após completar o 1º tabuleiro) 
             * verificar que buf[0] == '\0', libertar toda a memória e chamar readInputFile() 
             * recursivamente */
            printf("Existe outro tabuleiro neste ficheiro!\n");
            break; /* sai do while loop */
        }

        if (sscanf(buf, "%d %d %d", &(Wall->l1), &(Wall->c1), &(Wall->weight)) != 3)
            exit(EXIT_FAILURE);

        /* !TESTE! REMOVER DEPOIS */
        printf("Parede (%d, %d) com peso %d\n", Wall->l1, Wall->c1, Wall->weight);
        board[(Wall->l1 - 1) * brp->board.columns + Wall->c1 - 1] = Wall->weight;
    }

    /* inicia o jogo */
    initGameMode(brp, board);
    free(Wall);
    free(board);
    free(brp);
 
    /* verifica se há outro tabuleiro, se sim, chama readInputFile() recursivamente */
    if (buf[0] == '\n')
    {
        readInputFile(fp, brp);
    }

    return;
}
