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
struct List{
  // El hijo proximo
  struct List* next;
  // El ultimo hijo
  struct List* last;
  // El indice que guarda el tetraedro correspondiente
  //(Permite sacar las matrices, los vecref y los volumenes)
  int index;
  // Guarda la longitud de la lista (solo la guarda el primero, o ahi vemos)
  unsigned int n;
};
typedef struct List List;

/*
 * Arbol
 */
struct Nodo{
  // El padre
  //struct Nodo dad;
  // El hijo izquierdo
  struct Nodo* left;
  // El ultimo hijo
  struct Nodo* right;
  // La lista que guarda los tetraedros contenidos en el espacio definido por este nodo
  struct List thdrons;
  // El numero de tetraedros que hay de este nodo en adelante
  //unsigned int n;
  // Inicio del espacio
  float ini;
  // Final del espacio
  float fin;
  // La dimension de restriccion: es un entero   n%3
  //int dim;
};
typedef struct Nodo Nodo;

//------------------------------------------------------------------------------
//--- Metodos inicializadores---------------------------------------------------
//------------------------------------------------------------------------------

/*
 * Inicializa un nodo vacio
Nodo iniNodo(){
  Nodo nodo;
  nodo.left = &nodoI;
  nodo.right = &nodoD;
  nodo.thdrons = lista;
  nodo.n = n;
  nodo.ini = ini;
  nodo.fin = fin;
  return nodo;
}
*/

/*
 * Inicializa una lista vacía dado un índice;
 */
List* iniList( int index ){
  List *li = malloc(sizeof(List));
  li->next = 0;
  li->n = 1;
  li->index = index;
  li->last = li;
  return li;
}




/*
 * Concatena lista2 a lista
 */
void cat(List* lista, List* lista2){
  // Aumenta el numero de elementos en la lista primera.
  lista->n = lista->n + lista2->n;
  // Crea el elemento a la lista
  // Añade el nuevo elemento en la cola y cambia la referencia al elemento final en el primer nodo
  (lista->last)->next = lista2;
  lista->last = lista2->last;
}

/*
 * Crea un arbol, binario altura L
struct Nodo mkTree(int L,Nodo ini){
   if( L == 0){
     return ini;
   }
   else{
     L--;
     Nodo ini2 = ini;
     Nodo new;
     new.left = &ini;
     new.right = &ini2;
     return mkTree(L,new);
   }
}*/

void printList(List* lista){
  List *l = lista;
  do{
    printf("-->%d\n", l->index );
    l = l->next;
  }while(l != 0);
}


int main(){

  // Prueba de inilist
  List *lista1 = iniList(1);
  List *lista2 = iniList(3);
  printf("Lista1:\n");
  printList(lista1);
  printf("Lista2:\n");
  printList(lista2);
  printf("Longitud: L1=%d, L2=%d\n", lista1->n, lista2->n);

    ////////////////////////////////////////////////////////////////
   ////////////////////// IMPORTANTE!!!////////////////////////////
  ////////////////////////////////////////////////////////////////

  // Asi se pasa de un nodo a otro
  lista1 = lista1->last;

    ////////////////////////////////////////////////////////////////
   ////////////////////// IMPORTANTE!!!////////////////////////////
  ////////////////////////////////////////////////////////////////
  printf("Lista1:\n");
  printf("Longitud: L1=%d\n", lista1->n);
  printList(lista1);

  // Prueba del método cat
  cat(lista1,lista2);
  printf("Lista1:\n");
  printf("Longitud: L1=%d\n", lista1->n);
  printList(lista1);

  return 0;
}
