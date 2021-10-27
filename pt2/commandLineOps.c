#include <stdlib.h>
#include <string.h>
#include "commandLineOps.h"

/**
* @brief liberta a memória dinamicamente alocada para as strings input e output,
*        e finalmente liberta a memória acupada pela estrutura do tipo _commandLineOptions
* @note 
* @param *CLOps: estrutura do tipo _commandLineOptions
* @retval None
*/
void freeOps (cLineOps *CLOps)
{
    free(CLOps->input);
    free(CLOps->output);
    free(CLOps);
    return;
}

/**
* @brief recebe o nome do ficheiro de input e retorna o nome do ficheiro de output 
* SEM O NULL CARACTER que termina a string!!!
* @note 
* @param *CLOps: estrutura do tipo _commandLineOptions 
* @retval None
*/
void changeExt (cLineOps *CLOps)
{
    unsigned long int i = 0;

    for (i = 0; i < strlen(CLOps->input); i++)
        CLOps->output[i] = CLOps->input[i];
    CLOps->output[i] = '\0';

    if (CLOps->fase == 1)
    {
        CLOps->output[i - 3] = 's';
        CLOps->output[i - 2] = 'o';
        CLOps->output[i - 1] = 'l';
        CLOps->output[i] = '1';

    } if (CLOps->fase == 2) {
        CLOps->output[i - 2] = 's';
        CLOps->output[i - 1] = 'o';
        CLOps->output[i] = 'l';
    }

    return;
}

/*
* @brief consoante a fase do projeto preenche a variavel do tipo CLOps com as informações relevantes da fase, i.e.,
*  nome do ficheiro (com a extensão correta)
* @note 
* @param *CLOps: estrutura do tipo _commandLineOptions 
* @param **argv: argumentos de linha de comandos
* @retval None
*/
void genCLOps (cLineOps *CLOps, char **argv)
{
    if (CLOps->fase == 1)
    {
        if ((CLOps->input = (char *)malloc((strlen(argv[2]) + 1) * sizeof(char))) == NULL)
            exit(0);

        if ((CLOps->output = (char *)malloc((strlen(argv[2]) + 2) * sizeof(char))) == NULL)
            exit(0);

        strcpy(CLOps->input, argv[2]);
        changeExt(CLOps);
        /* é necessário acrescentar o null caracter no final da string visto que nome output > input */
        CLOps->output[strlen(argv[2]) + 1] = '\0';

    } else if (CLOps->fase == 2){
        if ((CLOps->input = (char *)malloc((strlen(argv[1]) + 1) * sizeof(char))) == NULL)
            exit(0);

        if ((CLOps->output = (char *)malloc((strlen(argv[1]) + 2) * sizeof(char))) == NULL)
            exit(0);

        strcpy(CLOps->input, argv[1]);
        changeExt(CLOps);
        /* é necessário acrescentar o null caracter no final da string visto que nome output > input */
        CLOps->output[strlen(argv[1]) + 1] = '\0';
    } 

    return;
}

/**
* @brief verifica, consoante a fase do projeto, se a extensão do ficheiro está correta
* @note 
* @param *str: string a ser testada 
* @param fase: fase do projeto
* @retval retorna a fase do projeto (int)
*/
int verifyExt (char *str, int fase)
{
    if (fase == 1)
    {
        if (str == NULL)
            exit(0);

        if (str[strlen(str) - 4] == '.' && str[strlen(str) - 3] == 'i' && str[strlen(str) - 2] == 'n' && str[strlen(str) - 1] == '1')
            return fase;

    } else if (fase == 2) {
        if (str == NULL)
            exit(0);

        if (str[strlen(str) - 3] == '.' && str[strlen(str) - 2] == 'i' && str[strlen(str) - 1] == 'n')
            return fase;
    }

    /* se o nome do ficheiro não terminar numa destas extensões, então termina o programa */
    exit(0); 
}

/**
* @brief verifica se o programa foi invocado corretamente, para além de verificar o modo de jogo 
*  e a extensão do ficheiro de entrada
* @note 
* @param **argv: argumentos da linha de comando 
* @param argc: contador dos argumentos de linha de comando 
* @param *CLOps: estrutura do tipo _commandLineOptions 
* @retval apontador para a estrutura do tipo _commandLineOptions 
*/
cLineOps *cLineOptions (char **argv, int argc, cLineOps *CLOps)
{
    if ((CLOps = (cLineOps *)malloc(sizeof(cLineOps))) == NULL)
        exit(0);  

    CLOps->fase = 0;
    CLOps->input = NULL;
    CLOps->output = NULL;

    if ((argc < 2) || (argc > 3))
        exit(0);

    if (strcmp(argv[1], "-s") != 0) {
        CLOps->fase = verifyExt(argv[1], 2);
        genCLOps(CLOps, argv);

    } else if (strcmp(argv[1], "-s") == 0) {
        CLOps->fase = verifyExt(argv[2], 1);
        genCLOps(CLOps, argv);

    } else exit(0);


    return CLOps;
}
