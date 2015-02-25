/*
 * Arbol ( con su respectiva lista )
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/*
 * Lista simplemente encadenada
 */
typedef struct List{
  // El hijo proximo
  struct List* next;
  // El ultimo hijo
  struct List* last;
  // El indice que guarda el tetraedro correspondiente
  //(Permite sacar las matrices, los vecref y los volumenes)
  int index;
  // Guarda la longitud de la lista (solo la guarda el primero, o ahi vemos)
  unsigned int n;
}List;

/*
 * Arbol
 */
typedef struct Nodo{
  // El padre
  //struct Nodo dad;
  // El hijo izquierdo
  struct Nodo* left;
  // El ultimo hijo
  struct Nodo* right;
  // La lista que guarda los tetraedros contenidos en el espacio definido por este nodo
  struct List* thdrons;
  // El numero de tetraedros que hay de este nodo en adelante
  unsigned int n;
  // Inicio del espacio
  float ini;
  // Final del espacio
  float fin;
  // La dimension de restriccion: es un entero   n%3
  //int dim;
}Nodo;

/*
 * Concatena 2 listas
 */
void cat(List lista,struct List lista2){
  // Aumenta el numero de elementos en la lista primera.
  lista.n = lista.n + lista2.n;
  // Crea el elemento a la lista
  // Añade el nuevo elemento en la cola y cambia la referencia al elemento final en el primer nodo
  lista.last.next = *lista2;
  lista.last = *lista2.last;
}

/*
 * Crea un arbol, binario altura L
 */
struct Nodo mkTree(int L,Nodo ini){
   if( L == 0){
     return ini;
   }
   else{
     L--;
     struct Nodo ini2 = ini;
     struct Nodo new =  {ini,ini2, NULL,0,0,0};
     new.left = &ini;
     new.right = &ini2;
     return mkTree(L,new);
   }
}

void printList(List lista){
  Lista l = lista;
  do{
    printf("-->%d\n", l.index );
    l = lista.next;
  }while(l != NULL);
}

int main(){
  struct List lista1 = {NULL,NULL, 1, 3};
  lista1.next = {NULL,NULL, 3, 3};
  struct Lista temp = {NULL,NULL, 5, 3};
  lista1.next.next = temp;
  lista1.last = temp;
  struct List lista2 = {NULL,NULL, 2, 3};
  lista2.next = {NULL,NULL, 4, 3};
  temp = {NULL,NULL, 6, 3};
  lista2.next.next = temp;
  lista2.last = temp;
  printList(lista1);
  printList(lista2);
  printList(cat(lista1,lista2));
  return 0;
}
