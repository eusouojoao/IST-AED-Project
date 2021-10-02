typedef struct edge Edge;
Edge *newEdge(int, int, int); //função que faz uma aresta entre dois numeros (recebe os parametrs do utilizador)

typedef struct graph Graph;

Graph *GraphInit(int);              //cria um grafo com um numero de arestas
void GraphInsertS(Graph *, Edge *); //insere uma aresta no grafo
//void GraphRemoveS(Graph *, Edge *); //não deve ser preciso remover quadrados
int GraphEdges(Edge **, Graph *); //diz o numero de quadrados e coloca as arestas num vetor
