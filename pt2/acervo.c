#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "acervo.h"

void acervoInit (acervo * A, int (* comparador) (int, int), int size )
{
    if (A == NULL)
        exit(0);
     
    A->acervoArray = (int *) malloc ( size * sizeof(int) );
    if (A->acervoArray == NULL)
        exit(0);
        
    A->max_size = A->current_size = 0;
    A->comparador = comparador;

    return;
}

void acervoInserirTopo (acervo *A, int cnt)
{
    int temp;
    int childs;
    int left = 0, right = 0;

    if ( (2 * cnt + 1) < A->current_size ) 
        left = 1;

    if ( (2 * cnt + 2) < A->current_size ) 
        right = 1;

     if (left && right) { 
         if (A->comparador(A->acervoArray[2 * cnt + 1], A->acervoArray[2 * cnt + 2]) < 0) {
             childs = 2 * cnt + 1;
         } else {
             childs = 2 * cnt + 2;
         }
     } else if (left) {
         childs = 2 * cnt + 1;
     } else if (right) {
         childs = 2 * cnt + 2;
     } else {
         return; 
     }

    if (A->comparador(A->acervoArray[ childs ], A->acervoArray[ cnt ]) < 0) { 
         temp = A->acervoArray[ cnt ];
         A->acervoArray[ cnt ] = A->acervoArray[ childs ];
         A->acervoArray[ childs ] = temp;
         acervoInserirTopo(A, childs); 
     }

    return;
}

 void acervoInserirFundo(acervo *A, int cnt) 
{
     int parent = (int) floor( (double)cnt / 2 );
     int temp;

     if (cnt == 0)
         return;

     if (A->comparador(A->acervoArray[ cnt ], A->acervoArray[ parent ]) < 0) 
     {
         temp = A->acervoArray[ cnt ];
         A->acervoArray[ cnt ] = A->acervoArray[ parent ];
         A->acervoArray[ parent ] = temp;
     }

     acervoInserirFundo(A, parent);
 }

 int acervoAdd (acervo *A, int it) 
{
     A->acervoArray[ A->current_size ] = it;
     acervoInserirFundo (A, A->current_size);
     A->current_size++;
     return 0;
 }

int acervoGet (acervo *A) 
{ 
     int min;

     if ( A->current_size == 0 ) {
         exit(0);
     }

     min = A->acervoArray[0];

     A->acervoArray[0] = A->acervoArray[ A->current_size - 1 ];
     A->current_size--;

     if (A->current_size != 1) {
         acervoInserirTopo (A, 0);
     }

     return min;
 }

 int acervoDelete (acervo *A, int cnt, int it) {
     int childs; 

     if ((cnt < 0) || (cnt >= A->current_size))
         return -1;

     if (A->comparador(it, A->acervoArray[ cnt ]) == 0) {
         A->acervoArray[ cnt ] = A->acervoArray[ A->current_size - 1 ];
         A->current_size--;
         acervoInserirTopo(A, cnt);
         return 0;
     }

     childs = 2 * cnt + 1;
     if (childs >= A->current_size) {
         return -1;
     }

     if (acervoDelete(A, childs, it) == 0 ) {
         return 0;
     }

     childs = 2 * (cnt + 1);
     if (childs >= A->current_size) {
         return -1;
     }

     if (acervoDelete(A, childs, it) == 0) {
         return 0;
     }

     return -1;
 }

int acervoEmpty (acervo *A)
{
    if (A->current_size != 0)
        return 0;
    return 1;
}
