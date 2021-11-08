#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "readFinalInputFile.h"
#include "common.h"
#include "grafo.h"
#include "algoritmo.h"
#include "divideRooms.h"
#include "assembleGraph.h"

#define FINAL
#include "writeOutputFile.h"

void experimental(FILE *fp, boardRules *brp, bool *valido, bool *especifico, bool first, char *output);
void experimentalPrint(Graph *G, bool *valido, bool *especifico, bool first);

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
    bool valido = 1, especifico = 0;
    wall *Wall = NULL;
    int *board = NULL, *wallVec = NULL;

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

    /*§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§*/
    /*                                EXPERIMENTAL                                */
    /*§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§*/

    experimental(fp, brp, &valido, &especifico, first, output);

    /*§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§*/
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

int getRooms(FILE *fp, bool *valido, bool *especifico, boardRules *brp)
{
    wall *Wall = (wall *)malloc(sizeof(wall));
    if (Wall == NULL)
        exit(0);

    int size = brp->board.columns, n_walls = brp->n_walls;
    bool old = 0; /* abstração, não importa se é 0 ou 1, o novo é sempre o complementar do old */
    int linha = 1, SALA = -2, x = 0;
    int a = 0, b = 0;
    bool salaNova = false;

    //---------------------------//
    int **arr = (int **)malloc(2 * sizeof(int *));
    if (arr == NULL)
    {
        free(Wall);
        exit(0);
    }

    arr[0] = (int *)calloc(size, sizeof(int));
    arr[1] = (int *)calloc(size, sizeof(int));
    if (arr[0] == NULL || arr[1] == NULL)
    {
        free(Wall);
        free(arr);
        exit(0);
    }

    //---------------------------//
    //---------------------------//
    if (fscanf(fp, "%d %d %d", &((Wall)->l1), &((Wall)->c1), &((Wall)->weight)) != 3)
    {
        exit(0);
    }
    n_walls--;

    if ((Wall->l1 == 1 && Wall->c1 == 1) || (Wall->l1 == brp->key.Line && Wall->c1 == brp->key.Column))
        *valido = 0;

    if (Wall->l1 != linha)
    {
        for (int i = 0; i < size; i++)
        {
            arr[old][i] = SALA;
        }

        linha = Wall->l1;
        SALA--;
    }
    arr[!old][Wall->c1 - 1] = Wall->weight;

    while (n_walls > 0)
    {
        while (linha == Wall->l1 && n_walls > 0)
        {
            if (fscanf(fp, "%d %d %d", &((Wall)->l1), &((Wall)->c1), &((Wall)->weight)) != 3)
            {
                exit(0);
            }

            n_walls--;

            if ((Wall->l1 == 1 && Wall->c1 == 1) || (Wall->l1 == brp->key.Line && Wall->c1 == brp->key.Column))
            {
                /* descartar as paredes deste tabuleiro invalido */
                while (n_walls > 0)
                    if (fscanf(fp, "%d %d %d", &((Wall)->l1), &((Wall)->c1), &((Wall)->weight)) != 3)
                        exit(0);

                /* garbage collector */
                free(Wall);
                free(arr[0]);
                free(arr[1]);
                free(arr);

                /* marcar como invalido e retornar para imprimir -1 como solução deste tabuleiro */
                *valido = 0;
                return (-1);
            }

            if (Wall->l1 == linha)
                arr[!old][Wall->c1 - 1] = Wall->weight;
        }

        if ((Wall->l1 - linha > 1))
        {
            for( a = b = 0 ; (a < size) && (b < size) ; /* * */ )
            {
                for ( a = b; a < size && arr[!old][a] != 0 ; a++);
                for ( b = a; b < size && arr[!old][b] == 0 ; b++ );
                x = 0;
                for (int i = a ; i < b ; i++)
                {
                    if ( (arr[old][i]) < -1 )
                    {
                        x = arr[old][i];
                        break; 
                    }
                }
                /* a unica forma de 'a' == 'b' é quando é uma sala de tamanho 1 com uma parede (I think) */
                if ( a != b )
                {
                    salaNova = false;
                    for (/* * */; a < b ; a++) 
                    {
                        if ( (arr[!old][a]) == 0 && x < -1)
                            arr[!old][a] = x;
                        else if ( (arr[!old][a]) == 0 && x >= -1 )
                        {
                            arr[!old][a] = SALA; 
                            salaNova = true;
                        }
                    }
                    if (salaNova) SALA--;
                } 
            }

            /* a nova linha passa a antiga */
            old = !old;

            for (int i = 0; i < size; i++)
            {
                arr[!old][i] = 0;
            }

            for( a = b = 0 ; (a < size) && (b < size) ; /* * */ )
            {
                for ( a = b; a < size && arr[!old][a] != 0 ; a++);
                for ( b = a; b < size && arr[!old][b] == 0 ; b++ );
                x = 0;
                for (int i = a; i < b ; i++)
                {
                    if ( (arr[old][i]) < -1 )
                    {
                        x = arr[old][i];
                        break; 
                    }
                }
                if ( a != b )
                {
                    salaNova = false;
                    for (/* * */; a < b ; a++) 
                    {
                        if ( (arr[!old][a]) == 0 && x < -1)
                            arr[!old][a] = x;
                        else if ( (arr[!old][a]) == 0 && x >= -1 )
                        {
                            arr[!old][a] = SALA;
                            salaNova = true;
                        }
                    }
                    if (salaNova) SALA--;
                } 
                else  
                {
                    if( ((arr[!old][a - 1]) == 0) && ((x = arr[old][a - 1]) < -1) )
                        arr[!old][a - 1] = x;
                    else if( ((arr[!old][a - 1]) == 0) && (x >= -1) )
                        arr[!old][a - 1] = SALA--;
                }
            }

            if (Wall->l1 != linha)
                arr[!old][Wall->c1 - 1] = Wall->weight;
        }
        else
        {
            for( a = b = 0 ; (a < size) && (b < size) ; /* * */ )
            {
                for ( a = b; a < size && arr[!old][a] != 0 ; a++);
                for ( b = a; b < size && arr[!old][b] == 0 ; b++ );
                x = 0;
                for (int i = a ; i < b ; i++)
                {
                    if ( (arr[old][i]) < -1 )
                    {
                        x = arr[old][i];
                        break;
                    }
                }
                /* a unica forma de 'a' == 'b' é quando é uma sala de tamanho 1 com uma parede (I think) */
                if ( a != b )
                {
                    salaNova = false;
                    for (/* * */; a < b ; a++) 
                    {
                        if ( (arr[!old][a]) == 0 && x < -1)
                            arr[!old][a] = x;
                        else if ( (arr[!old][a]) == 0 && x >= -1 )
                        {
                            arr[!old][a] = SALA; 
                            salaNova = true;
                        }
                    }
                    if (salaNova) SALA--;
                } 
            }

           /* a nova linha passa a antiga */
            old = !old;

            for (int i = 0; i < size; i++)
            {
                arr[!old][i] = 0;
            }

            if (Wall->l1 != linha)
                arr[!old][Wall->c1 - 1] = Wall->weight;
        }

        linha = Wall->l1;
    }

    //---------------------------//
    //---------------------------//
 
    if (linha != brp->board.lines)
    {
        for( a = b = 0 ; (a < size) && (b < size) ; /* * */ )
        {
            for ( a = b; a < size && arr[!old][a] != 0 ; a++);
            for ( b = a; b < size && arr[!old][b] == 0 ; b++ );
            x = 0;
            for (int i = a ; i < b ; i++)
            {
                if ( (arr[old][i]) < -1 )
                {
                    x = arr[old][i];
                    break; 
                }
            }
            /* a unica forma de 'a' == 'b' é quando é uma sala de tamanho 1 com uma parede (I think) */
            if ( a != b )
            {
                salaNova = false;
                for (/* * */; a < b ; a++) 
                {
                    if ( (arr[!old][a]) == 0 && x < -1)
                    {
                        arr[!old][a] = x;
                    }
                    else if ( (arr[!old][a]) == 0 && x >= -1 )
                    {
                        arr[!old][a] = SALA; 
                        salaNova = true;
                    }
                }
                if (salaNova) SALA--;
            } 
        }

        /* garbage collector */
        free(Wall);
        free(arr[0]);
        free(arr[1]);
        free(arr);

        return (-SALA - 2);
    }
    //---------------------------//

    /* garbage collector */
    free(Wall);
    free(arr[0]);
    free(arr[1]);
    free(arr);

    return (-SALA - 2);
}

void pensar ( Graph *G, int *backup, int *old , int *new, int size, int linha )
{
    for ( int i = 0, a = 0, b = 0 ; i < size ; i++ )
    {
        if ( old[i] != -1 )
        {
            /* nos cantos só podemos verificar em cima e em baixo... */
            if ( (i == 0) || (i == size - 1) )
            {
                if ( old[i] < -1 )
                {
                    if ( ( (a = backup[i]) < -1) && ( (b = new[i]) < -1 ) && (a != b) )
                    {
                        insertInGraph (G, a, b, 0, 0);
                    }
                }
                else if ( old[i] > 0 )
                {
                    if ( ( (a = backup[i]) < -1) && ( (b = new[i]) < -1 ) && (a != b) )
                    {
                        insertInGraph (G, a, b, convertTile(linha-1, i+1, size), old[i]);
                    }                   
                }
            }
            else
            {
                if ( old[i] < -1 )
                {
                    if ( ( (a = backup[i]) < -1) && ( (b = new[i]) < -1 ) && (a != b) )
                    {
                        insertInGraph (G, a, b, 0, 0);
                    }
                    else if ( ( (a = old[i-1]) < -1) && ( (b = old[i+1]) < -1 ) && (a != b) )
                    {
                        insertInGraph (G, a, b, 0, 0);
                    }
                }
                else if ( old[i] > 0 )
                {
                    if ( ( (a = backup[i]) < -1) && ( (b = new[i]) < -1 ) && (a != b) )
                    {
                        insertInGraph (G, a, b, convertTile(linha-1, i+1, size), old[i]);
                    } 
                    else if ( ( (a = old[i-1]) < -1) && ( (b = old[i+1]) < -1 ) && (a != b) )
                    {
                        insertInGraph (G, a, b, convertTile(linha-1, i+1, size), old[i]);
                    }
                }
            }
        }
    }

    return;
}

void getGraph(Graph *myGraph, FILE *fp, bool *valido, bool *especifico, boardRules *brp)
{
    wall *Wall = (wall *)malloc(sizeof(wall));
    if (Wall == NULL)
        exit(0);

    int size = brp->board.columns, n_walls = brp->n_walls;
    bool old = 0; /* abstração, não importa se é 0 ou 1, o novo é sempre o complementar do old */
    int linha = 1, SALA = -2, x = 0;
    int a = 0, b = 0;
    bool salaNova = false;

    //---------------------------//
    int **arr = (int **)malloc(2 * sizeof(int *));
    if (arr == NULL)
    {
        free(Wall);
        exit(0);
    }

    int *backup = (int *)calloc(size, sizeof(int));
    if (backup == NULL)
    {
        free(Wall);
        free(arr);
        exit(0);
    }

    arr[0] = (int *)calloc(size, sizeof(int));
    arr[1] = (int *)calloc(size, sizeof(int));
    if (arr[0] == NULL || arr[1] == NULL)
    {
        free(Wall);
        free(arr);
        exit(0);
    }

    //---------------------------//
    //---------------------------//
    if (fscanf(fp, "%d %d %d", &((Wall)->l1), &((Wall)->c1), &((Wall)->weight)) != 3)
    {
        exit(0);
    }
    n_walls--;

    if ((Wall->l1 == 1 && Wall->c1 == 1) || (Wall->l1 == brp->key.Line && Wall->c1 == brp->key.Column))
        *valido = 0;

    if (Wall->l1 != linha)
    {
        for (int i = 0; i < size; i++)
        {
            arr[old][i] = SALA;
        }

        linha = Wall->l1;
        SALA--;
    }
    arr[!old][Wall->c1 - 1] = Wall->weight;

    while (n_walls > 0)
    {
        while (linha == Wall->l1 && n_walls > 0)
        {
            if (fscanf(fp, "%d %d %d", &((Wall)->l1), &((Wall)->c1), &((Wall)->weight)) != 3)
            {
                exit(0);
            }

            n_walls--;

            if ((Wall->l1 == 1 && Wall->c1 == 1) || (Wall->l1 == brp->key.Line && Wall->c1 == brp->key.Column))
            {
                /* descartar as paredes deste tabuleiro invalido */
                while (n_walls > 0)
                    if (fscanf(fp, "%d %d %d", &((Wall)->l1), &((Wall)->c1), &((Wall)->weight)) != 3)
                        exit(0);

                /* garbage collector */
                free(Wall);
                free(arr[0]);
                free(arr[1]);
                free(arr);

                /* marcar como invalido e retornar para imprimir -1 como solução deste tabuleiro */
                *valido = 0;
                return ;
            }

            if (Wall->l1 == linha)
                arr[!old][Wall->c1 - 1] = Wall->weight;
        }


        if ((Wall->l1 - linha > 1))
        {
            for( a = b = 0 ; (a < size) && (b < size) ; /* * */ )
            {
                for ( a = b; a < size && arr[!old][a] != 0 ; a++);
                for ( b = a; b < size && arr[!old][b] == 0 ; b++ );
                x = 0;
                for (int i = a ; i < b ; i++)
                {
                    if ( (arr[old][i]) < -1 )
                    {
                        x = arr[old][i];
                        break; 
                    }
                }
                /* a unica forma de 'a' == 'b' é quando é uma sala de tamanho 1 com uma parede (I think) */
                if ( a != b )
                {
                    salaNova = false;
                    for (/* * */; a < b ; a++) 
                    {
                        if ( (arr[!old][a]) == 0 && x < -1)
                            arr[!old][a] = x;
                        else if ( (arr[!old][a]) == 0 && x >= -1 )
                        {
                            arr[!old][a] = SALA; 
                            salaNova = true;
                        }
                    }
                    if (salaNova) SALA--;
                } 
            }

            pensar( myGraph, backup, arr[old], arr[!old], size, linha);

            for (int i = 0; i < size; i++ )
                backup[i] = arr[old][i];

            /* a nova linha passa a antiga */
            old = !old;

            for (int i = 0; i < size; i++)
            {
                arr[!old][i] = 0;
            }

            for( a = b = 0 ; (a < size) && (b < size) ; /* * */ )
            {
                for ( a = b; a < size && arr[!old][a] != 0 ; a++);
                for ( b = a; b < size && arr[!old][b] == 0 ; b++ );
                x = 0;
                for (int i = a; i < b ; i++)
                {
                    if ( (arr[old][i]) < -1 )
                    {
                        x = arr[old][i];
                        break; 
                    }
                }
                if ( a != b )
                {
                    salaNova = false;
                    for (/* * */; a < b ; a++) 
                    {
                        if ( (arr[!old][a]) == 0 && x < -1)
                            arr[!old][a] = x;
                        else if ( (arr[!old][a]) == 0 && x >= -1 )
                        {
                            arr[!old][a] = SALA;
                            salaNova = true;
                        }
                    }
                    if (salaNova) SALA--;
                } 
                else  
                {
                    if( ((arr[!old][a - 1]) == 0) && ((x = arr[old][a - 1]) < -1) )
                        arr[!old][a - 1] = x;
                    else if( ((arr[!old][a - 1]) == 0) && (x >= -1) )
                        arr[!old][a - 1] = SALA--;
                }
            }

            if (Wall->l1 != linha)
                arr[!old][Wall->c1 - 1] = Wall->weight;
        }
        else
        {
            for( a = b = 0 ; (a < size) && (b < size) ; /* * */ )
            {
                for ( a = b; a < size && arr[!old][a] != 0 ; a++);
                for ( b = a; b < size && arr[!old][b] == 0 ; b++ );
                x = 0;
                for (int i = a ; i < b ; i++)
                {
                    if ( (arr[old][i]) < -1 )
                    {
                        x = arr[old][i];
                        break;
                    }
                }
                /* a unica forma de 'a' == 'b' é quando é uma sala de tamanho 1 com uma parede (I think) */
                if ( a != b )
                {
                    salaNova = false;
                    for (/* * */; a < b ; a++) 
                    {
                        if ( (arr[!old][a]) == 0 && x < -1)
                            arr[!old][a] = x;
                        else if ( (arr[!old][a]) == 0 && x >= -1 )
                        {
                            arr[!old][a] = SALA; 
                            salaNova = true;
                        }
                    }
                    if (salaNova) SALA--;
                } 
            }

            pensar( myGraph, backup, arr[old], arr[!old], size, linha);

            for (int i = 0; i < size; i++ )
                backup[i] = arr[old][i];

            /* a nova linha passa a antiga */
            old = !old;

            for (int i = 0; i < size; i++)
            {
                arr[!old][i] = 0;
            }

            if (Wall->l1 != linha)
                arr[!old][Wall->c1 - 1] = Wall->weight;
        }

        linha = Wall->l1;
    }

    //---------------------------//
    //---------------------------//
 
    if (linha != brp->board.lines)
    {
        for( a = b = 0 ; (a < size) && (b < size) ; /* * */ )
        {
            for ( a = b; a < size && arr[!old][a] != 0 ; a++ );
            for ( b = a; b < size && arr[!old][b] == 0 ; b++ );
            x = 0;
            for (int i = a ; i < b ; i++)
            {
                if ( (arr[old][i]) < -1 )
                {
                    x = arr[old][i];
                    break; 
                }
            }
            /* a unica forma de 'a' == 'b' é quando é uma sala de tamanho 1 com uma parede (I think) */
            if ( a != b )
            {
                salaNova = false;
                for (/* * */; a < b ; a++) 
                {
                    if ( (arr[!old][a]) == 0 && x < -1)
                    {
                        arr[!old][a] = x;
                    }
                    else if ( (arr[!old][a]) == 0 && x >= -1 )
                    {
                        arr[!old][a] = SALA; 
                        salaNova = true;
                    }
                }
                if (salaNova) SALA--;
            } 

            pensar( myGraph, backup, arr[old], arr[!old], size, linha);
        }

        /* garbage collector */
        free(Wall);
        free(arr[0]);
        free(arr[1]);
        free(arr);

        return ;
    }
    //---------------------------//

    /* garbage collector */
    free(Wall);
    free(arr[0]);
    free(arr[1]);
    free(arr);

    return ;
}

void experimental(FILE *fp, boardRules *brp, bool *valido, bool *especifico, bool first, char *output)
{
    Graph *myGraph = NULL;

    //---------------------------//
    int offset = ftell(fp);                               /* já lemos as boardRules, marcar posição de leitura das paredes */
    int maxRooms = getRooms(fp, valido, especifico, brp); /* obter numero majorado de salas (vértices) do grafo            */
    printf("%d\n", maxRooms);

    //exit(1); // <- está a sair aqui para testes!!! comentar para prosseguir


    /* marcar deslocamento do fp, após ter lido as paredes */
    offset -= ftell(fp);

    printf("valido = %d\nespecifico = %d\n", *valido, *especifico);
    if ( (*valido) && !(*especifico) )
    {
        /* rewind ao fp o deslocamento, para ler as paredes denovo */
        if (fseek(fp, offset, SEEK_CUR) != 0)   
        {
            exit(0);
        }
    //---------------------------//
    /* -> criar o grafo e chamar o algoritmo (se necessário) <-*/
    myGraph = graphInit(maxRooms);
    getGraph(myGraph, fp, valido, especifico, brp);
    printGraph(myGraph);

    /* descobrir caminho */
    //algoritmo(myGraph);
    }
    //---------------------------//
    //---------------------------//

    /* escreve para o ficheiro de saída */
    //writeSolution(output, myGraph, 0, brp->board.columns, first);
    graphDestroy(myGraph);

    //experimentalPrint(myGraph, valido, especifico, first);

    //---------------------------//
    return;
}

void experimentalPrint(Graph *G, bool *valido, bool *especifico, bool first)
{
    return;
}
