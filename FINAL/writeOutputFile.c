#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "grafo.h"
#include "writeOutputFile.h"
#include "algoritmo.h"

/**
* @brief Função que faz a escrita para o ficheiro de output no formato pedido 
* @note 
* @param *output: nome do ficheiro de output
* @param to_print: valor a ser escrito no ficheiro 
* @retval None
*/ 
void write2outputFile (char *output, int to_print)
{
    FILE *fp = fopen(output, "a");
    if (fp == NULL)
        exit(0);

    fprintf(fp, "%d\n\n", to_print);

    fclose(fp);
    return;
}

void writeZero (char *output, bool first)
{
    FILE *fp = fopen(output, "a");
    if (fp == NULL)
        exit(0);

    if (!first)
        fprintf(fp, "\n\n");

    fprintf(fp,"0\n");
    fclose(fp);

    return;
}

void writeInvalid (char *output, bool first)
{
    FILE *invfp = fopen(output, "a");
    if (invfp == NULL)
        exit(0);

    if (!first)
        fprintf(invfp, "\n\n");

    fprintf(invfp, "-1\n");
    fclose(invfp);
    return;
}
