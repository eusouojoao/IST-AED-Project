typedef struct square Square;
Square *newSquare(int, int); //função que faz uma aresta entre dois numeros (recebe os parametrs do utilizador)

typedef struct graph Graph;

//typedef struct edge Edge;

Graph *graphInit(int, int, int, int); //cria um grafo com um numero de arestas
void graphInsertS(Graph *, Square *); //insere uma aresta no grafo

//void GraphRemoveS(Graph *, Edge *); //não deve ser preciso remover quadrados
//int graphEdges(Edge **, Graph *); //diz o numero de quadrados e coloca as arestas num vetor