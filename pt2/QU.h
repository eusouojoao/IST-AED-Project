#ifndef __QU__
#define __QU__

void QuickUnion(int *id, int p, int q);
void connectivityAdjacentQU(int *id, int *board, int tile, int n_Lines, int n_Col);
int sameRoomQU(int *board, int n_Lines, int n_Col, int p1, int p2);

#endif
