#ifndef __Common__
#define __Common__

int isTileAdjacent(int tile, int n_Lines, int n_Col, int HouV);
void inicializeBoard(int *board, int n_Lines, int n_Col);
int checkInsideBoard(int n_Lines, int n_Col, int l, int c);
int checkPeca(int *board, int n_Lines, int n_Col, int l, int c);
int checkBreakable(int *board, int n_Lines, int n_Col, int l, int c);
int checkSameRoom(int *board, int n_Lines, int n_Col, int p1, int p2);
int convertTile(int l, int c, int n_Col);
void pushAdjacent(int *board, int tile, int n_Lines, int n_Col);
int adjacentTile(int l, int c, int p2L, int p2C);
int checkAA(int *AA, int mode, int n_adj);
int checkA5(int **A5, int n_adj, int n_Lines, int n_Col, int l1, int c1);

#endif
