#include <stdio.h>
#include <stdlib.h>

void write2outputFile (char *output, int to_print)
{
    static int p = 0;
    FILE *fp = fopen(output, "a");

    if (fp == NULL)
        exit(0);

    if (p != 0)
        fprintf(fp, "\n\n");

    fprintf(fp, "%d", to_print);
    p++;

    fclose(fp);
    return;
}
