//IGNORAR POR AGORA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "graph.h"

int convert(int l, int c, int n_Lines, int n_Col)
{
    int aux = (l - 1) * n_Col + c - 1;
    return aux;
}

void leituraF(FILE *fp)
{
    int n_V = 0, n_Col = 0, n_Lines = 0, key_Line = -1, key_Col = -1, n_walls = 0, l = 0, c = 0, w = 0;
    char variante[2];
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

    //Edge **edgesArray;
    Graph *myGraph;
    Square *newS;
    myGraph = graphInit(n_V, n_Lines, n_Col, n_walls);
    while (fscanf(fp, "%d %d %d", &l, &c, &w) != EOF)
    {
        printf("parede em %d %d com custo %d\n", l, c, w);
        newS = newSquare(convert(l, c, n_Lines, n_Col), w);
        graphInsertS(myGraph, newS);
        //newB = newBlock(l, c, w);
        //GraphInsertB(myGraph, newB);
        //edgesArray[i++] = newEdge(l, c, w);                //criar  vetor de arestas ESTÁ MAL
    }
    int posCheck = convert(3, 2, n_Lines, n_Col);
    int tileType = checkTile(myGraph, posCheck);
    printf("Na posição %d está uma celula de custo %d\n", posCheck, tileType);

    posCheck = convert(3, 3, n_Lines, n_Col);
    tileType = checkTile(myGraph, posCheck);
    printf("Na posição %d está uma celula de custo %d\n", posCheck, tileType);

    int mode = 3, modeTest = -1;
    posCheck = convert(3, 2, n_Lines, n_Col);
    modeTest = checkAdjacent(myGraph, posCheck, mode);
    if (modeTest == 0)
        printf("A celula %d não tem peças cinzentas adjacentes", posCheck);
    else if (modeTest == 1)
        printf("A celula %d tem peças cinzentas adjacentes", posCheck);
}
