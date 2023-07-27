#ifndef __modosA__
#define __modosA__

int adjacentTileLC (int l, int c, int p2L, int p2C);
int checkAA(int *AA, int mode, int n_adj);
int checkA5(int **A5, int n_adj, int n_Lines, int n_Col, int l1, int c1, int tesouro);
int checkA6(int *board, int n_Lines, int n_Col, int p1, int p2);

#endif
