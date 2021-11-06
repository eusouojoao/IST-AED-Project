#ifndef __dividir__
#define __dividir__

void inicializeWallVector(int *wallVec, int n_walls);
int divideRooms(int *board, int n_Lines, int n_Col, int tesouroL, int tesouroC);
void pushTiles(int *board, int tile, int n_Lines, int n_Col, int room);

#endif
