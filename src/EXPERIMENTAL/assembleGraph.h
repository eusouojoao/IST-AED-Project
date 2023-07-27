#ifndef __assembleGraph__
#define __assembleGraph__

void insertInGraph(Graph *myGraph, int room1, int room2, int p_wall, int weight_wall);
void fillGraph(Graph *myGraph, int *board, int *wallVec, int n_walls, int n_Lines, int n_Col);

#endif
