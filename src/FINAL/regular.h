#ifndef __regular__
#define __regular__

void allocs(FILE *fp, boardRules *brp, int **wallVec, int **board, bool *valido, bool *especifico);
void init(boardRules *brp, int *board, int *wallVec, bool valido, bool especifico, bool first, char *output);

#endif
