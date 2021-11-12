#ifndef __readInputFileFinal__
#define __readInputFileFinal__

typedef struct _boardRules boardRules;
void readFinalInputFile(FILE *fp, boardRules *brp, char *output);
/* funções auxiliares */
int getWalls(boardRules *brp);
int getBoardLines(boardRules *brp);
int getBoardColumns(boardRules *brp);
int getKeyLine(boardRules *brp);
int getKeyColumn (boardRules *brp);

#endif
