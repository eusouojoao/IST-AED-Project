#ifndef __CommandLineOptions__
#define __CommandLineOptions__

/*typedef struct _commandLineOptions cLineOps;*/
typedef struct _commandLineOptions 
{
    int fase;
    char *input, *output;
} cLineOps;

cLineOps *cLineOptions (char **argv, int argc, cLineOps *CLOps);
void checkCLOps (cLineOps *CLOps);
void freeOps (cLineOps *CLOps);

#endif
