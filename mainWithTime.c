#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include "header.h"

int main(int argc, char *argv[])
{
    clock_t begin = clock();
    double time_spent = 0.0;

    if (argc < 2)
        return 1;
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        return 1;

    leituraP(fp);

    fclose(fp);
    clock_t end = clock();

    // calculate elapsed time by finding difference (end - begin) and
    // dividing the difference by CLOCKS_PER_SEC to convert to seconds
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    printf("The elapsed time is %f seconds\n", time_spent);
    return 0;
}