#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "readInputFile.h"
#include "commandLineOps.h"

int main(int argc, char *argv[])
{
    boardRules *brp = NULL;
    cLineOps *CLOps = NULL;
    //clock_t begin = clock();
    //double time_spent = 0.0;

    CLOps = cLineOptions(argv, argc, CLOps);
    //printf("%s\n", CLOps->input);
    //printf("%s\n", CLOps->output);

    FILE *fp = fopen(CLOps->input, "r");
    if (fp == NULL)
        return 0;

    readInputFile(fp, brp, CLOps->output);
    fclose(fp);
    freeOps(CLOps);

    //clock_t end = clock();
    // calculate elapsed time by finding difference (end - begin) and
    // dividing the difference by CLOCKS_PER_SEC to convert to seconds
    //time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    //printf("The elapsed time is %f seconds\n", time_spent);

    return 0;
}
