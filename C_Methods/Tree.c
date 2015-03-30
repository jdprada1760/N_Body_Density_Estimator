/*
 * Arbol ( con su respectiva lista )
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define xmin 0.0
#define ymin 0.0
#define zmin 0.0
#define xmax 150000.0
#define ymax 150000.0
#define zmax 150000.0

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
  struct List* thdrons;
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
 * Inicializa un nodo vacio con los límites dados
 */
Nodo* iniNodo(float ini, float fin){
  Nodo *nodo = malloc(sizeof(Nodo));
  nodo->left = 0;
  nodo->right = 0;
  nodo->thdrons = 0;
  nodo->ini = ini;
  nodo->fin = fin;
  return nodo;
}

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
 * Añade un nuevo elemento a la lista
 */
void add(List* lista, unsigned int index){
  // Aumenta el numero de elementos en la lista primera.
  List *li = malloc(sizeof(List));
  li->next = 0;
  li->n = 1;
  li->index = index;
  li->last = 0;
  lista->n = lista->n +1;
  // Crea el elemento a la lista
  // Añade el nuevo elemento en la cola y cambia la referencia al elemento final en el primer nodo
  (lista->last)->next = li;
  lista->last = li;
}

/*
 * Imprime una lista
 */
void printList(List* lista){
  List *l = lista;
  do{
    printf("-->%d\n", l->index );
    l = l->next;
  }while(l != 0);
}

Nodo* iniTree(int orden){
  Nodo* Tree = iniNodo(0,0);
  if(orden == 0){
    return Tree;
  }
  orden--;
  Tree->left = iniTree(orden);
  Tree->right = iniTree(orden);
  return Tree;
}

int main(){
  /*
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


  // Prueba del método add to list
  List *lista5 = iniList(0);
  int i;
  for(i = 1; i < 21; i++){
    add(lista5,i);
  }
  printf("Lista5:\n");
  printf("Longitud: L5=%d\n", lista5->n);
  printList(lista5);

  cat(lista1,lista5);
  printf("Lista1:\n");
  printf("Longitud: L1=%d\n", lista1->n);
  printList(lista1);
  */
  /*
  Nodo* Tree = iniNodo(xmin,xmax);
  // Primer Piso
  Tree->left = iniNodo(xmin,xmax/2);
  Tree->right = iniNodo(xmax/2,xmax);
  // Segundo piso
  Tree->left->left = iniNodo(ymin,ymax/2);
  Tree->left->right = iniNodo(ymax/2,ymax);

  Tree->right->left = iniNodo(ymin,ymax/2);
  Tree->right->right = iniNodo(ymax/2,ymax);
  // Tercer Piso
  Tree->left->left->left = iniNodo(zmin,zmax/2);
  Tree->left->left->right = iniNodo(zmax/2,zmax);

  Tree->left->right->left = iniNodo(zmin,zmax/2);
  Tree->left->right->right = iniNodo(zmax/2,zmax);


  Tree->right->left->left = iniNodo(zmin,zmax/2);
  Tree->right>left->right = iniNodo(zmax/2,zmax);

  Tree->right->right->left = iniNodo(zmin,zmax/2);
  Tree->right->right->right = iniNodo(zmax/2,zmax);
  */
  Nodo* Tree = iniTree(3);

  Tree->left->left->left->thdrons = iniList(2);
  Tree->right->left->left->thdrons = iniList(5);

  printf("LLL: %d\n",Tree->left->left->left->thdrons->index);
  printf("RLL: %d\n",Tree->right->left->left->thdrons->index);
  return 0;
}
