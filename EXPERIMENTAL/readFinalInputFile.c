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
void experimentalPrint(char *output, Graph *G, int salaDoTesouro, bool *valido, bool *especifico, bool first, boardRules *brp);

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
        printf("ola\n");
        readFinalInputFile(fp, brp, output);
    }

    return;
}

int getRooms(FILE *fp, bool *valido, boardRules *brp)
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
    if (arr[0] == NULL)
    {
        free(Wall);
        free(arr);
        exit(0);
    }
    arr[1] = (int *)calloc(size, sizeof(int));
    if (arr[0] == NULL)
    {
        free(Wall);
        free(arr[0]);
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
                {
                    if (fscanf(fp, "%d %d %d", &((Wall)->l1), &((Wall)->c1), &((Wall)->weight)) != 3)
                    {
                        exit(0);
                    }
                    n_walls--;
                }
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
                for ( a = b; a < size && arr[!old][a] != 0 ; a++ );
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

void pensar ( Graph *G, int *backup, int *old , int *new, int size, int linha, int salaDoTesouro )
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
                    if ( ((a = backup[i]) < -1) && ((b = old[i]) < -1) && (a != b) )
                    {
                        //insertInGraph (G, a, b, 0, 0);
                        mergeRooms(G, -a -2, -b -2, salaDoTesouro);
                    }
                    else if ( ( (a = backup[i]) < -1) && ( (b = new[i]) < -1 ) && (a != b) )
                    {
                        //insertInGraph (G, a, b, 0, 0);
                    }
                }
                else if ( old[i] > 0 )
                {
                    if ( ( (a = backup[i]) < -1) && ( (b = new[i]) < -1 ) && (a != b) )
                    {
                        insertInGraph (G, a, b, convertTile(linha, i+1, size), old[i]);
                    }                   
                }
            }
            else
            {
                if ( old[i] < -1 )
                {
                    if ( ((a = backup[i]) < -1) && ((b = old[i]) < -1) && (a != b) )
                    {
                        //insertInGraph (G, a, b, 0, 0);
                        mergeRooms(G, -a -2, -b -2, salaDoTesouro);
                    }
                    else if ( ( (a = backup[i]) < -1) && ( (b = new[i]) < -1 ) && (a != b) )
                    {
                        //insertInGraph (G, a, b, 0, 0);
                        mergeRooms(G, -a -2, -b -2, salaDoTesouro);
                    }
                    else if ( ( (a = old[i-1]) < -1) && ( (b = old[i+1]) < -1 ) && (a != b) )
                    {
                        //insertInGraph (G, a, b, 0, 0);
                        mergeRooms(G, -a -2, -b -2, salaDoTesouro);
                    }
                }
                else if ( old[i] > 0 )
                {
                    if ( ( (a = backup[i]) < -1) && ( (b = new[i]) < -1 ) && (a != b) )
                    {
                        insertInGraph (G, a, b, convertTile(linha, i+1, size), old[i]);
                    } 
                    else if ( ( (a = old[i-1]) < -1) && ( (b = old[i+1]) < -1 ) && (a != b) )
                    {
                        insertInGraph (G, a, b, convertTile(linha, i+1, size), old[i]);
                    }
                }
            }
        }
    }

    return;
}

int getGraph(Graph *G, FILE *fp, bool *valido, boardRules *brp)
{
    int size = brp->board.columns, n_walls = brp->n_walls;
    int linha = 1, SALA = -2, x = 0;
    int a = 0, b = 0, salaDoTesouro = -1;
    bool old = 0; /* abstração, não importa se é 0 ou 1, o novo é sempre o complementar do old */
    bool salaNova = false, salaTesouroFound = false, ler = true, flag = true;

    //---------------------------//
    wall *Wall = (wall *)malloc(sizeof(wall));
    if (Wall == NULL)
        exit(0);


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

    if ( (arr[0] = (int *)calloc(size, sizeof(int))) == NULL )
    {
        free(Wall);
        free(arr);
        exit(0);
    }

    if( (arr[1] = (int *)calloc(size, sizeof(int))) == NULL )
    {
        free(Wall);
        free(arr[0]);
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

        if ( !salaTesouroFound && ( brp->key.Line >= linha ) && (brp->key.Line <= Wall->l1) )
        {
            salaDoTesouro = -(arr[old][0]) - 2 ;
            printf("tesouro = %d\n", salaDoTesouro);
            salaTesouroFound = true;
        }

        linha = Wall->l1;
        SALA--;
    }

    arr[!old][Wall->c1 - 1] = Wall->weight;

    while (n_walls > 0)
    {
        while (linha == Wall->l1 && n_walls > 0)
        {
            ler = false;
            if (fscanf(fp, "%d %d %d", &((Wall)->l1), &((Wall)->c1), &((Wall)->weight)) != 3)
            {
                exit(0);
            }

            n_walls--;

            if (Wall->l1 == linha)
                arr[!old][Wall->c1 - 1] = Wall->weight;
        }


        if ((Wall->l1 - linha > 1))
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
                        if ( (arr[!old][a]) == 0 && x < -1 )
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

            pensar( G, backup, arr[old], arr[!old], size, linha, salaDoTesouro );

            /* a nova linha passa a antiga */
            old = !old;

            for (int i = 0; i < size; i++)
            {
                backup[i] = arr[!old][i];
                arr[!old][i] = 0;
            }

            for( a = b = 0 ; (a < size) && (b < size) ; /* * */ )
            {
                for ( a = b; a < size && arr[!old][a] != 0 ; a++ );
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
                        if ( (arr[!old][a]) == 0 && x < -1 )
                            arr[!old][a] = x;
                        else if ( (!arr[old][a]) == 0 && x >= -1 )
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

                printf("l629; sala = %d\n", salaDoTesouro);

            pensar( G, backup, arr[old], arr[!old], size, linha, salaDoTesouro );

            for (int i = 0; i < size; i++ )
            {
                backup[i] = arr[old][i];
                arr[old][i] = arr[!old][i];
            }   

            if ( !salaTesouroFound && ( brp->key.Line >= linha ) && (brp->key.Line <= Wall->l1) )
            {
                salaDoTesouro = -(arr[old][0]) - 2 ;
                salaTesouroFound = true;
            }


            if (Wall->l1 != linha)
                arr[!old][Wall->c1 - 1] = Wall->weight;
        }
        else
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
                        if ( (arr[!old][a]) == 0 && x < -1 )
                            arr[!old][a] = x;
                        else if ( (arr[!old][a]) == 0 && x >= -1 )
                        {
                            arr[!old][a] = SALA; 
                            salaNova = true;
                        }
                    }
                    if (salaNova) SALA--;
                } 

                pensar( G, backup, arr[old], arr[!old], size, linha-1, salaDoTesouro );
            }


            if ( !salaTesouroFound && ( brp->key.Line == linha ) )
            {
                salaDoTesouro = -(arr[!old][(brp->key.Column - 1)]) - 2 ;
                printf("l668,sala = %d\n", salaDoTesouro);
                salaTesouroFound = true;
            }

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
        ler = true;
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

        }

        pensar( G, backup, arr[old], arr[!old], size, linha-1, salaDoTesouro );

        if (ler)
        {
            if (salaNova)
            {
                for (int i = 0; i < size; i++ )
                {
                    backup[i] = arr[old][i];
                    arr[old][i] = arr[!old][i] = SALA + 1;
                }
            }
            else
                for (int i = 0; i < size; i++ )
                {
                    backup[i] = arr[old][i];
                    arr[old][i] = arr[!old][i] = x;
                }
        }


        if ( !salaTesouroFound && ( brp->key.Line >= linha ) && (brp->key.Column <= brp->board.columns) )
        {
            salaDoTesouro = -(arr[!old][(brp->key.Column - 1)]) - 2 ;
            salaTesouroFound = true;
        }
    }
    else
    {
        x = 0;
        for (int i = 0; i < size; i++)
            if (arr[!old][i] > 0)
                x++;

        if (x != 1)
            flag = false;
        else
            flag = true;

        if(flag)
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
                        if ( (arr[!old][a]) == 0 && x < -1 )
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

            for (int i = 0; i < size; i++)
                backup[i] = arr[old][i];

            old = !old;

            if ( !salaTesouroFound && ( brp->key.Line >= linha ) && (brp->key.Column <= brp->board.columns) )
            {
                salaDoTesouro = -(arr[!old][(brp->key.Column - 1)]) - 2 ;
                salaTesouroFound = true;
            }

            for (int i = 0; i < size; i++)
                arr[!old][i] = -1;
        }
        else
        {
            for(int i = 0; i < size; i++)
                arr[!old][i] = -1;
        }
/*
        printf("backup = ");
        for(int i = 0; i < size; i++)
            printf("%d ", backup[i]);
        printf("\n");
        printf("old = ");
        for(int i = 0; i < size; i++)
            printf("%d ", arr[old][i]);
        printf("\n");
        printf("new = ");
        for(int i = 0; i < size; i++)
            printf("%d ", arr[!old][i]);
        printf("\n");
*/
        pensar( G, backup, arr[old], arr[!old], size, linha, salaDoTesouro );

    }
    //---------------------------//

    /* garbage collector */
    free(Wall);
    free(arr[0]);
    free(arr[1]);
    free(arr);

    return salaDoTesouro;
}

void experimental(FILE *fp, boardRules *brp, bool *valido, bool *especifico, bool first, char *output)
{
    Graph *G = NULL;
    int salaDoTesouro = 0;
    //---------------------------//
    int offset = ftell(fp);                               /* já lemos as boardRules, marcar posição de leitura das paredes */
    int maxRooms = getRooms(fp, valido, brp); /* obter numero majorado de salas (vértices) do grafo            */

    /* marcar deslocamento do fp, após ter lido as paredes */
    offset -= ftell(fp);

    if ( (*valido) && !(*especifico) )
    {
        /* rewind ao fp o deslocamento, para ler as paredes denovo */
        if (fseek(fp, offset, SEEK_CUR) != 0)   
        {
            exit(0);
        }
    //---------------------------//
    /* -> criar o grafo e chamar o algoritmo (se necessário) <-*/
    G = graphInit(maxRooms);
    salaDoTesouro = getGraph(G, fp, valido, brp);
    //printGraph(G);
    
    //exit(1);
    /* descobrir caminho */
    if (salaDoTesouro >= 0)
        algoritmo(G);
    }
    //---------------------------//
    //---------------------------//

    /* escreve para o ficheiro de saída */
    experimentalPrint(output, G, salaDoTesouro, valido, especifico, first, brp);
    if ( (*valido) && !(*especifico) )
        graphDestroy(G);

    //---------------------------//
    return;
}

void experimentalPrint(char *output, Graph *G, int salaDoTesouro, bool *valido, bool *especifico, bool first, boardRules *brp)
{
    if ( (*valido) && !(*especifico) )
    {
        writeSolution(output, G, salaDoTesouro, brp->board.columns, first);
    }
    else
    {
        if (*especifico)
            writeZero(output, first);
        else
            writeInvalid(output, first);
    }

    return;
}
