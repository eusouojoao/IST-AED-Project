#ifndef __experimental__
#define __experimental__

void experimental(FILE *fp, boardRules *brp, bool *valido, bool *especifico, bool first, char *output, bool expFlag);
void experimentalPrint(char *output, Graph *G, int salaDoTesouro, bool *valido, bool *especifico, bool first, int colunas, bool expFlag);

#endif
