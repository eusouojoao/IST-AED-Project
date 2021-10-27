typedef struct edge Edge;
Edge *newEdge(int, int, int, int); //função que faz uma aresta entre duas salas

typedef struct graph Graph;

Graph *graphInit(int);              //cria um grafo com um numero de vertices
void graphInsertE(Graph *, Edge *); //insere uma aresta no grafo

//int graphEdges(Edge **, Graph *); //diz o numero de quadrados e coloca as arestas num vetor

void graphDestroy(Graph *);

void printGraph(Graph *);