#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "readFinalInputFile.h"
#include "grafo.h"
#include "algoritmo.h"
#include "assembleGraph.h"
#include "experimental.h"
#include "common.h"

#define FINAL
#include "writeOutputFile.h"

/* parede (l1, c1) com valor weight */
typedef struct _wall
{
    int l1, c1, weight;
} wall;

/**
 * @brief   Processa a linha "old", i.e., verifica se salas etão conectadas com custo 0,
 *          ou se a parede no array "old" é quebravel, e se for, cria uma conecção entre as salas
 *          respetivas com o custo da tal parede no array "old"
 * @note
 * @param   *G: apontador para o grafo
 * @param   *backup: array mais antigo (em termos de idade)
 * @param   *old: array antigo (a ser processado)
 * @param   *new: array novo (apenas com informações novas)
 * @param   size: tamanho máximo dos arrays (número de colunas do tabuleiro)
 * @param   linha: linha correspondente ao array "old"
 * @param   salaDoTesouro: indica a sala do tesouro em número negativo, i.e., antes de ser convertido
 * @retval None
 */
void pensar ( Graph *G, int *backup, int *old , int *new, int size, int linha)
{       
    for ( int i = 0, a = 0, b = 0 ; i < size ; i++ )
    {
        if ( old[i] != -1 )
        {
            /* nos cantos só podemos verificar em cima e em baixo... não vale a pena verificar para os lados */
            if ( (i == 0) || (i == size - 1) )
            {
                if ( old[i] < -1 )
                {
                    /* verificar se estamos numa sala e se na linha anterior está uma sala diferente */
                    if ( ((a = backup[i]) < -1) && ((b = old[i]) < -1) && (a != b) )
                    {
                        insertInGraph (G, a, b, 0, 0);
                    }
                    /* verificar se na linha atual e na linha seguinte estão salas diferentes */
                    else if ( ((a = new[i]) < -1) && ((b = old[i]) < -1) && (a != b) )
                    {
                        insertInGraph (G, a, b, 0, 0);

                    }
                    /* verificar se na linha atual anterior e na linha seguinte estão salas diferentes  */
                    else if ( ( (a = backup[i]) < -1) && ( (b = new[i]) < -1 ) && (a != b) )
                    {
                        insertInGraph (G, a, b, 0, 0);
                    }
                }
                else if ( old[i] > 0 )
                {
                    /* verificar se na linha anterior e na linha seguinte estão salas diferentes, 
                     * se sim, é quebravel e junta as duas salas 
                     * */
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
                    /* verificar se estamos numa sala e se na linha anterior está uma sala diferente */
                    if ( ((a = backup[i]) < -1) && ((b = old[i]) < -1) && (a != b) )
                    {
                        insertInGraph (G, a, b, 0, 0);
                    }
                    /* verificar se na linha anterior e na linha seguinte estão salas diferentes  */
                    else if ( ( (a = backup[i]) < -1) && ( (b = new[i]) < -1 ) && (a != b) )
                    {
                        insertInGraph (G, a, b, 0, 0);
                    }
                    /* verificar se nas colunas adjacentes estão salas diferentes */
                    else if ( ( (a = old[i-1]) < -1) && ( (b = old[i+1]) < -1 ) && (a != b) )
                    {
                        insertInGraph (G, a, b, 0, 0);
                    }                   
                    /* verificar se a linha atual e a linha seguinte têm salas diferentes */
                    else if ( ((a = new[i]) < -1) && ((b = old[i]) < -1) && (a != b) )
                    {
                        insertInGraph (G, a, b, 0, 0);
                    }

                }
                else if ( old[i] > 0 )
                {
                    /* verificar se na linha anterior e na linha seguinte estão salas diferentes,
                     * caso seja afirmativo, estamos a ver uma parede quebravel e unimos as salas verticalmente
                     * */
                    if ( ( (a = backup[i]) < -1) && ( (b = new[i]) < -1 ) && (a != b) )
                    {
                        insertInGraph (G, a, b, convertTile(linha, i+1, size), old[i]);
                    } 
                    /* verificar se nas colunas adjacentes estão salas diferentes,
                     * caso se verifique é uma parede quebravel horizontalmente
                     * */
                    if ( ( (a = old[i-1]) < -1) && ( (b = old[i+1]) < -1 ) && (a != b) )
                    {
                        insertInGraph (G, a, b, convertTile(linha, i+1, size), old[i]);
                    }
                }
            }
        }
    }

    return;
}

/**
 * @brief  Função que lê as paredes do ficheiro de entrada de modo a majorarmos o tamanho grafo
 * @note   Obtem o número majorado de salas do tabuleiro
 * @param  *fp: apontador para o ficheiro de entrada
 * @param  *valido: flag booleana passada por referencia; indica se o tabuleiro é válido
 * @param  *brp: boardRules pointer, contem toda a informação relevante sobre as regras do tabuleiro
 * @retval número majorado de salas (int)
 */
int getRooms(FILE *fp, bool *valido, boardRules *brp)
{
    /* limites */
    int size = getBoardColumns(brp), n_walls = getWalls(brp);
    bool old = 0; /* abstração, não importa se é 0 ou 1, o novo é sempre o complementar do old */
    /* variaveis auxiliares */
    int linha = 1, SALA = -2;
    int a = 0, b = 0, x = 0;
    bool salaNova = false;

    /* inicializar dinamicamente os vetores */
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

    /* inicializar as 2 linhas a serem analizadas */
    arr[0] = (int *)calloc(size, sizeof(int));
    if (arr[0] == NULL)
    {
        free(Wall);
        free(arr);
        exit(0);
    }
    arr[1] = (int *)calloc(size, sizeof(int));
    if (arr[1] == NULL)
    {
        free(Wall);
        free(arr[0]);
        free(arr);
        exit(0);
    }

    //---------------------------//
    //---------------------------//
    /* ler a primeira parede */
    if (fscanf(fp, "%d %d %d", &((Wall)->l1), &((Wall)->c1), &((Wall)->weight)) != 3)
    {
        free(Wall);
        exit(0);
    }
    n_walls--;

    /* se a parede estiver em cima da posição inicial, ou em cima do tesouro, o tabuleiro passa a inválido */
    if ((Wall->l1 == 1 && Wall->c1 == 1) || (Wall->l1 == getKeyLine(brp) && Wall->c1 == getKeyColumn(brp)))
        *valido = 0;

    /* se a linha da primeira parede lida não for 1, então podemos assumir que existe uma sala antes */
    if (Wall->l1 != linha)
    {
        /* pintar a linha antiga com a sala */
        for (int i = 0; i < size; i++)
        {
            arr[old][i] = SALA;
        }

        linha = Wall->l1;
        SALA--;
    }

    /* preencher o array novo com a primeira parede lida */
    arr[!old][Wall->c1 - 1] = Wall->weight;

    while (n_walls > 0)
    {
        /* enquanto estivermos a ler paredes da mesma linha */
        while (linha == Wall->l1 && n_walls > 0)
        {
            if (fscanf(fp, "%d %d %d", &((Wall)->l1), &((Wall)->c1), &((Wall)->weight)) != 3)
            {
                exit(0);
            }

            n_walls--;

            if ( !valido || (Wall->l1 == 1 && Wall->c1 == 1) || (Wall->l1 == getKeyLine(brp) && Wall->c1 == getKeyColumn(brp)))
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


        /* se houver desfazamento entre a linha completamente lida (no array novo) 
         * e a proxima linha que será lida então podemos assumir que existe uma sala
         * comum a ambas (que as separa)
         * */
        if ((Wall->l1 - linha > 1))
        {
            /* "pintar" o array novo com as sala do array old */
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

            /* inicializar a linha seguinte a zeros para ser pintada */
            for (int i = 0; i < size; i++)
            {
                arr[!old][i] = 0;
            }

            /* pintar a linha nova que foi inicializada a zeros, 
             * de modo a processarmos a linha que terminamos de ler no while loop interior
             * */
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

            /* preencher o array novo com a parede mais recente que tem desfazamento em linhas superior a 1 */
            if (Wall->l1 != linha)
                arr[!old][Wall->c1 - 1] = Wall->weight;
        }
        else
        {
            /* "pintar" o array novo com as sala do array old */
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

            /* inicializar o array novo a zeros */
            for (int i = 0; i < size; i++)
            {
                arr[!old][i] = 0;
            }

            /* preencher o array novo com a parede mais recente que terminou o while loop interior */
            if (Wall->l1 != linha)
                arr[!old][Wall->c1 - 1] = Wall->weight;
        }

        linha = Wall->l1;
    }

    //---------------------------//
    //---------------------------//
    /* se a linha da ultima parede lida não corresponder ao limite de linhas do tabuleiro,
     * então devemos criar uma sala acima 
     * */
    if (linha != getBoardLines(brp))
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

        return (-SALA - 2); /* número de salas convertido em indices de C */
    }
    //---------------------------//

    /* garbage collector */
    free(Wall);
    free(arr[0]);
    free(arr[1]);
    free(arr);

    return (-SALA - 2); /* número de salas convertido em indices de C */
}

/**
 * @brief Lê as paredes do ficheiro de entrada e cria o grafo sequencialmente
 * @note
 * @param *G: apontador para o grafo
 * @param *fp: apontador para o ficheiro de leitura
 * @param *valido: apontador para booleano que indica a validade do tabuleiro
 * @param *brp: apontador para boardRules indica as caracteristicas do tabuleiro (i.e., dimensões L e C, coordenadas do tesouro...)
 * @retval salaDoTesouro em valor negativo segundo a "pintura" (int) 
 */
int getGraph(Graph *G, FILE *fp, bool *valido, boardRules *brp)
{
    /* limites */
    int size = getBoardColumns(brp), n_walls = getWalls(brp);
    bool old = 0; /* abstração, não importa se é 0 ou 1, o novo é sempre o complementar do old */
    /* flags auxiliares */
    bool salaNova = false, salaTesouroFound = false, ler = true, flag = true;
    /* variaveis auxiliares */
    int a = 0, b = 0, salaDoTesouro = -1;
    int linha = 1, SALA = -2, x = 0;

    /* iniciar dinamicamente os vetores */
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

    /* inicializar as 3 linhas a serem analizadas */
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
        free(backup);
        free(arr);
        exit(0);
    }

    if( (arr[1] = (int *)calloc(size, sizeof(int))) == NULL )
    {
        free(Wall);
        free(backup);
        free(arr[0]);
        free(arr);
        exit(0);
    }

    //---------------------------//
    //---------------------------//
    /* ler a primeira parede */
    if (fscanf(fp, "%d %d %d", &((Wall)->l1), &((Wall)->c1), &((Wall)->weight)) != 3)
    {
        exit(0);
    }
    n_walls--;

    /* se a parede estiver em cima da posição inicial, ou em cima do tesouro, o tabuleiro passa a inválido */
    if ((Wall->l1 == 1 && Wall->c1 == 1) || (Wall->l1 == getKeyLine(brp) && Wall->c1 == getKeyColumn(brp)))
        *valido = 0;

    /* se a linha da primeira parede lida não for 1, então podemos assumir que existe uma sala antes */
    if (Wall->l1 != linha)
    {
        /* pintar a linha antiga com a sala */
        for (int i = 0; i < size; i++)
        {
            arr[old][i] = SALA;
        }

        /* verificar se a sala do tesouro está entre 1 e a coordenada linha da primeira parede lida */
        if ( !salaTesouroFound && ( getKeyLine(brp) >= linha ) && (getKeyLine(brp) <= Wall->l1) )
        {
            salaDoTesouro = -(arr[old][0]) - 2 ;
            salaTesouroFound = true;
        }

        /* atualizar a linha, para que a atual seja a linha da parede e 
         * atualizar a "tinta" que vai "pintar" a próxima sala
         * */
        linha = Wall->l1;
        SALA--;
    }

    /* preencher o array novo com a primeira parede lida */
    arr[!old][Wall->c1 - 1] = Wall->weight;

    while (n_walls > 0)
    {
        /* enquanto estivermos a ler paredes da mesma linha */
        while (linha == Wall->l1 && n_walls > 0)
        {
            ler = false; /* indica se saímos por este while loop ou pelo while loop superior */
            if (fscanf(fp, "%d %d %d", &((Wall)->l1), &((Wall)->c1), &((Wall)->weight)) != 3)
            {
                exit(0);
            }

            n_walls--;

            /* preencher o array novo com paredes da linha correspondente */
            if (Wall->l1 == linha)
                arr[!old][Wall->c1 - 1] = Wall->weight;
        }

        /* se houver desfazamento entre a linha completamente lida (no array novo) 
         * e a proxima linha que será lida então podemos assumir que existe uma sala
         * comum a ambas (que as separa)
         * */
        if ((Wall->l1 - linha > 1))
        {
            /* "pintar" o array novo com as sala do array old */
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

            /* processar o array old de modo a obtermos os vértices e arestas do grafo */
            pensar( G, backup, arr[old], arr[!old], size, linha-1);

            /* a nova linha passa a antiga */
            old = !old;

            /* copiar a linha nova para o backup (linha mais antiga que a old)
             * pois existe uma sala que separa a linha atual e a linha da proxima parede
             * */
            for (int i = 0; i < size; i++)
            {
                backup[i] = arr[!old][i];
                arr[!old][i] = 0;
            }

            /* pintar a linha nova que foi inicializada a zeros, 
             * de modo a processarmos a linha que terminamos de ler no while loop interior
             * */
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

            /* processar o array old de modo a obtermos os vértices e arestas do grafo */
            pensar( G, backup, arr[old], arr[!old], size, linha);

            for (int i = 0; i < size; i++ )
            {
                backup[i] = arr[old][i];
                arr[old][i] = arr[!old][i];
            }   

            /* verificar se a linha do tesouro está contida neste alcance de coordenadas */
            if ( !salaTesouroFound && ( getKeyLine(brp) >= linha ) && (getKeyLine(brp) <= Wall->l1) )
            {
                salaDoTesouro = -(arr[old][0]) - 2 ;
                salaTesouroFound = true;
            }


            /* preencher o array novo com a parede mais recente que tem desfazamento em linhas superior a 1 */
            if (Wall->l1 != linha)
                arr[!old][Wall->c1 - 1] = Wall->weight;
        }
        else
        {
            /* "pintar" o array novo com as sala do array old */
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

                /* processar o array old de modo a obtermos os vértices e arestas do grafo */
                pensar( G, backup, arr[old], arr[!old], size, linha-1);
            }

            /* verificar se a linha da sala do tesouro é a linha atual */
            if ( !salaTesouroFound && ( getKeyLine(brp) == linha ) )
            {
                salaDoTesouro = -(arr[!old][(getKeyColumn(brp) - 1)]) - 2 ;
                salaTesouroFound = true;
            }

            /* copiar o array antigo para o array mais antigo */
            for (int i = 0; i < size; i++ )
                backup[i] = arr[old][i];

            /* a nova linha passa a antiga */
            old = !old;

            /* inicializar o array novo a zeros */
            for (int i = 0; i < size; i++)
            {
                arr[!old][i] = 0;
            }

            /* preencher o array novo com a parede mais recente que terminou o while loop interior */
            if (Wall->l1 != linha)
                arr[!old][Wall->c1 - 1] = Wall->weight;
        }

        /* avançar a linha e dar reset à flag que nos indica por onde saímos */
        linha = Wall->l1;
        ler = true;
    }

    //---------------------------//
    //---------------------------//
    /* se a linha da ultima parede lida não corresponder ao limite de linhas do tabuleiro,
     * então devemos criar uma sala acima e processar a linha correspondente às ultimas paredes lidas
     * */
    if (linha != getBoardLines(brp))
    {
        /* "pintar" o array novo com as sala do array old */
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

        /* processar o array old de modo a obtermos os vértices e arestas do grafo */
        pensar( G, backup, arr[old], arr[!old], size, linha);

        /* se saímos pelo while loop superior */
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

        /* se a linha da sala do tesoro estiver compreendida entre a linha atual e o fim do tabuleiro */
        if ( !salaTesouroFound && ( getKeyLine(brp) >= linha ) && (getKeyColumn(brp) <= getBoardColumns(brp)) )
        {
            salaDoTesouro = -(arr[!old][(getKeyColumn(brp) - 1)]) - 2 ;
            salaTesouroFound = true;
        }
    }
    /* processar a ultima linha do tabuleiro que tem paredes(!) */
    else
    {
        x = 0;
        /* varrer a ultima linha do tabuleiro (array novo) */
        for (int i = 0; i < size; i++)
            if (arr[!old][i] > 0)
                x++;

        /* de acordo com as paredes encontradas */
        if (x != 1)
            flag = false; /* neste caso o array novo ja vem "pintado" e está no array old (o novo está todo a zeros, porque trocamos os indices) */ 
        else
            flag = true; /* se só foi encontrada uma parede na ultima linha, então temos de a "pintar" */

        if(flag)
        {
            /* pintar o array novo */
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

            /* passar a linha anterior para a mais antiga */
            for (int i = 0; i < size; i++)
                backup[i] = arr[old][i];

            /* trocar a linha nova para que esta passe a ser a antiga */
            old = !old;

            if ( !salaTesouroFound && ( getKeyLine(brp) >= linha ) && (getKeyColumn(brp)<= getBoardColumns(brp)) )
            {
                salaDoTesouro = -(arr[!old][(getKeyColumn(brp) - 1)]) - 2 ;
                salaTesouroFound = true;
            }

            /* criar linha "falsa" fora do tabuleiro a -1 */
            for (int i = 0; i < size; i++)
                arr[!old][i] = -1;
        }
        else
        {
            /* criar linha nova (fora do tabuleiro) a -1 para processarmos a ultima linha do tabuleiro
             * que tem paredes
             * */
            for(int i = 0; i < size; i++)
                arr[!old][i] = -1;
        }

        /* processar o array old de modo a obtermos os vértices e arestas do grafo */
        pensar( G, backup, arr[old], arr[!old], size, linha);

    }
    //---------------------------//

    /* garbage collector */
    free(backup);
    free(Wall);
    free(arr[0]);
    free(arr[1]);
    free(arr);

    return salaDoTesouro;
}

/**
 * @brief Função que inicializa o processamento do ficheiro pelo método experimental que consome muito poucos
 *        recursos (memória)
 * @note
 * @param *fp: apontador para o ficheiro de entrada
 * @param *brp: apontador para boardRules (estrutura com as espeficicações do tabuleiro)
 * @param *valido: apontador para booleano com a validade do tabuleiro
 * @param *especifico: apontador para flag booleana que indica se estamos perante um tabuleiro especifico (ver função que avalia estes casos) 
 * @param first: flag booleana que indica se este é o primeiro tabuleiro lido do ficheiro de entrada
 * @param *output: string com o nome do ficheiro de output
 * @retval None
 */
void experimental(FILE *fp, boardRules *brp, bool *valido, bool *especifico, bool first, char *output, bool expFlag)
{
    Graph *G = NULL;
    int salaDoTesouro = 0;
    //---------------------------//
    int offset = ftell(fp);                   /* já lemos as boardRules, marcar posição de leitura das paredes */
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
        
        /* descobrir caminho */
        if (salaDoTesouro >= 0)
            algoritmo(G);

        //---------------------------//
        /* escreve para o ficheiro de saída */
        experimentalPrint(output, G, salaDoTesouro, valido, especifico, first, getBoardColumns(brp), expFlag);
        graphDestroy(G);
    }
    //---------------------------//

    return;
}

/**
 * @brief Função que escreve no ficheiro de saída conforme o algoritmo experimental que obtem o grafo
 * @note
 * @param *output: string que contem o nome do ficheiro de saída
 * @param *G: apontador para o grafo
 * @param salaDoTesouro: sala do tesouro convertida para indice em conformidade com a linguagem C
 * @param *valido: flag que indica a validade do tabuleiro
 * @param *especifico: flag que indica se estamos perante um tabuleiro especifico (ver função respetiva)
 * @param first: flag que indica se estamos no primeiro tabuleiro do ficheiro de entrada
 * @param colunas: número máximo de colunas do tabuleiro
 * @retval
 */
void experimentalPrint(char *output, Graph *G, int salaDoTesouro, bool *valido, bool *especifico, bool first, int colunas, bool expFlag)
{
    if ( (*valido) && !(*especifico) )
    {
        writeSolution(output, G, salaDoTesouro, colunas, first, expFlag);
    }
    else
    {
        if (*especifico && *valido)
            writeZero(output, first);
        else
            writeInvalid(output, first);
    }

    return;
}
