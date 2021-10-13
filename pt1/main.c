#include <stdio.h>
#include <stdlib.h>
#include "readInputFile.h"
#include "commandLineOps.h"

/**
 * @brief  
 * @note   
 * @param  argc: 
 * @param  *argv[]: 
 * @retval 
 */
int main(int argc, char *argv[])
{
    boardRules *brp = NULL;
    cLineOps *CLOps = NULL;

    CLOps = cLineOptions(argv, argc, CLOps);

    FILE *fp = fopen(CLOps->input, "r");
    if (fp == NULL)
        return 0;

    readInputFile(fp, brp, CLOps->output);

    fclose(fp);
    freeOps(CLOps);
    return 0;
}
