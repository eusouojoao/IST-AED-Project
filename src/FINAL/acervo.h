#ifndef __acervo__
#define __acervo__

typedef struct _acervo acervo;
void acervoInit(acervo **A, int (*comparador)(int, int), int size);
void acervoAdd(acervo *A, int it);
int acervoEmpty(acervo *A);
int acervoGet(acervo *A);
void freeAcervo (acervo *A);

#endif /* __acervo__ */
