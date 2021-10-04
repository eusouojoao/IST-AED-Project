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
    leitura(fp);
    //char c;
    /*
    fscanf(fp, "%d %d", &n_Lines, &n_Col);
    fscanf(fp, "%d %d", &key_Line, &key_Col);
    fscanf(fp, "%s", variante);
    n_V = n_Col * n_Lines;
    printf("numero de vertices é %d\n", n_V);
    printf("o tesouro está em %d %d\n", key_Line, key_Col);
    if ((strcmp(variante, "A1") == 0) || (strcmp(variante, "A2") == 0) || (strcmp(variante, "A3") == 0) || (strcmp(variante, "A4") == 0) || (strcmp(variante, "A5") == 0) || (strcmp(variante, "A6") == 0))
    {
        printf("a variante é %s\n", variante);
    }
    else
    {
        rewind(fp);
        fscanf(fp, "%*[^\n]\n*");
        fscanf(fp, "%*[^\n]\n*");
    }
    fscanf(fp, "%d", &n_walls);
    printf("numero total de paredes é %d\n", n_walls);
    //   int vector[n_walls][3];
    // while (fscanf(fp, "%d %d %d", &vector[n_walls][0], &vector[n_walls][1], &vector[n_walls][2]) != EOF)
    int l, c, w;
    while (fscanf(fp, "%d %d %d", &l, &c, &w) != EOF)
    {
        printf("parede em %d %d com custo %d\n", l, c, w); //newEdge(s, l, w);
    } */
    /*int i, j;
        for (i = 0; i < n_walls; i++)
    {-v
        for (j = 0; j < 3; j++)
            printf("%d ", vector[i][j]);
        printf("\n");
    } */
    /*while ((c = fgetc(fp)) != EOF)
    {
        if (c == ' ' || c == '\n')
        {
            printf("\n");
        }
        else
        {
            printf("%c", c);
        }
    } */
    printf("\n");
    fclose(fp);

    return 0;
}