#ifndef __board__
#define __board__

void inicializeBoard(int *board, int n_Lines, int n_Col);
void printBoard(int *board, int n_Lines, int n_Col);
int checkInsideBoard(int n_Lines, int n_Col, int l, int c);
int checkPeca(int *board, int n_Lines, int n_Col, int l, int c);
int checkBreakable(int *board, int n_Lines, int n_Col, int l, int c);
int checkAdjacencia(int *board, int n_Lines, int n_Col, int l, int c, int mode);
int checkSameRoom(int *board, int n_Lines, int n_Col, int p1, int p2);
int convertTile(int l, int c, int n_Col);
int sameRoomQU(int *board, int n_Lines, int n_Col, int p1, int p2);
int sameRoomWQU(int *board, int n_Lines, int n_Col, int p1, int p2);

#endif
