#ifndef __common__
#define __common__

int checkInsideBoard(int n_Lines, int n_Col, int l, int c);
int convertTile(int l, int c, int n_Col);
void inicializeBoard(int *board, int n_Lines, int n_Col);
int adjacentTileP (int tileP, int n_Lines, int n_Col, int HouV);

#endif
