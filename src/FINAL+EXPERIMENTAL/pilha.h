#ifndef pilha
#define pilha
#include "item.h"

void initPilha(unsigned pNumb);
void push(Item pI);
Item pop();
int isEmpty();
void freePilha();
#endif
