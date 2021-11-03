#ifndef __acervo__
#define __acervo__

/* EXPLICAR A STRUCT?*/
typedef struct _acervo
{
    int *acervoArray;
    int max_size;
    int current_size;
    int (*comparador)(int, int);
} acervo;

void acervoInit(acervo *A, int (*comparador)(int, int), int size);
int acervoAdd(acervo *A, int it);
int acervoDelete(acervo *A, int cnt, int it);
int acervoEmpty(acervo *A);
int acervoGet(acervo *A);

#endif /* __acervo__ */
