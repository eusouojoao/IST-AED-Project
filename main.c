#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "header.h"

int main(int argc, char *argv[])
{
    /*int n_V = 0, n_Col = 0, n_Lines = 0, key_Line = -1, key_Col = -1, n_walls = 0;
    char variante[2];*/
    if (argc < 2)
        return 1;
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        return 1;
    //leitura(fp);

    leituraP(fp);

    fclose(fp);

    return 0;
}