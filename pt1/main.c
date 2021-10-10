#include <stdio.h>
#include <stdlib.h>
/*#include "graph.h"*/
#include "header.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 1;
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        return 1;

    leituraP(fp);

    fclose(fp);

    return 0;
}
