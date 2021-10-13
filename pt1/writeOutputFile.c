#include <stdio.h>
#include <stdlib.h>

void write2outputFile (char *output, int to_print)
{
    FILE *fp = fopen(output, "a");
    if (fp == NULL)
        exit(0);

    fprintf(fp, "%d\n\n", to_print);

    fclose(fp);
    return;
}
