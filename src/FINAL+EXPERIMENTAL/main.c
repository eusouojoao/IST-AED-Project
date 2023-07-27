#include <stdio.h>
#include <stdlib.h>
#include "readInputFile.h"
#include "readFinalInputFile.h"
#include "commandLineOps.h"

int main(int argc, char *argv[])
{
    boardRules *brp = NULL;
    cLineOps *CLOps = NULL;

    CLOps = cLineOptions(argv, argc, CLOps);

    FILE *fp = fopen(CLOps->input, "r");
    if (fp == NULL)
        return 0;

    ( (CLOps->fase == 1) ? 
     (readInputFile(fp, brp, CLOps->output)) : (readFinalInputFile(fp, brp, CLOps->output)) );


    fclose(fp);
    freeOps(CLOps);
    return 0;
}
