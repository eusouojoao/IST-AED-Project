#include <stdio.h>
#include <stdlib.h>

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
