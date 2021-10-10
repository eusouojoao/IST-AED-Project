#include "Item.h"

typedef enum
{
    FALSE,
    TRUE
} BOOL;
void initPilha(unsigned pNumb); //parametro é o comprimento máximo
//int Empty();
void push(Item pI);
Item pop();
int isEmpty();

//função que desaloca memoria da stack
