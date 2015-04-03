/*
 * Programa que imprime la densidad de una simulacion dada por fstate en un conjunto de puntos dados. (Sin recurrir a una estructura
 * de datos para minimizar el problema)
 * Para incluir la libreria de algebra lineal: cc filename.c -o out.x libLinalg.a
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
// final state: el estado final de la simulacion en formato no binario
// points: El archivo con los puntos donde se quiere estimar la densidad
// towrite: El archivo final con la densidad en dichos puntos
#define man "./estimar.x finalState points toWrite"
#define nTot 1.0*pow(128,3)
#define nTh 6.0*pow(127,3)
#define mass 1.0

//----------------------------------------
//   Arbol ( Y su respectiva lista )
//----------------------------------------
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
  //unsigned int n;
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
  // La dimension de restriccion: es un entero   n%3
  //int dim;
};
typedef struct Nodo Nodo;
Nodo* Tree;

//----------------------------------------
//   Declaracion Metodos
//----------------------------------------
void allocate_All();
void readFile(FILE* data);
void readFile2(FILE* data);
void mkThdrons();
void getDensities();
void writeFile(FILE * data);
void add_to_Tree(int p1, int p2, int p3, int p4, int g);

// Linalg
float* product(float* m1, float* b1);
float* cross(float* a1,float* b1);
float det(float* m1);
float* inverse(float* m1);

// Arbol
Nodo* iniNodo();
List* iniList( int index );
Nodo* iniTree( int orden );
void add(List* lista, unsigned int index);
void printList(List* lista);


//----------------------------------------
//   Variables generales
//----------------------------------------

// Los valores que definen un tetraedro
float** matrices;
float** refVecs;
float* volumes;
// El estado final de la simulacion
float** fstate;
unsigned int* Ids;
// Contenedoras de los puntos a samplear 3*i +j
float** points;
unsigned int npoints;
float* densities;
// Numero total de tetraedros (Sin contar los tetraedros degenerados)
unsigned int realnTh;
// Los límites de la caja
float* rmin;
float* rmax;
int main(int argc, char **argv){
  // Inicializa los límites de la caja
  rmin = malloc(3*sizeof(float));
  rmax = malloc(3*sizeof(float));
  rmin[0] = 0.0;
  rmin[1] = 0.0;
  rmin[2] = 0.0;
  rmax[0] = 150000.0;
  rmax[1] = 150000.0;
  rmax[2] = 150000.0;
  // time_t start = time(NULL);
  // EL archivo donde se guardaron las condiciones finales y los puntos a samplear
  FILE *data;
  FILE *LoLpoints;
  FILE *toWrite;
  char *nameData = argv[1];
  char *namePoints = argv[2];
  char *nametoWrite = argv[3];
  // Aparta memoria
  allocate_All();
  // El archivo de texto de condiciones iniciales y de evolucion en el tiempo
  data = fopen( nameData, "r" );
  // Lee el archivo data a fstate
  readFile(data);
  fclose(data);
  // inicializa el arbol de tetraedros antes de llenarlo
  Tree = iniTree(3);
  // Crea los tetraedros definidos por matrices, refvecs y volumes y los mete en el arbol

  mkThdrons();

  // Desaloja la memoria utilizada por fstate y Ids
  free(Ids);
  int i;
  for( i = 0; i < nTot; i++){
    free(fstate[i]);
  }
  free(fstate);
  //printf("Total Time elapsed: %f\n", (double)(time(NULL) - start));
  // Lee los puntos para evaluar la densidad
  LoLpoints = fopen( namePoints, "r" );
  readFile2(LoLpoints);
  fclose(LoLpoints);
  getDensities();
  // Imprime los puntos en un archivo
  toWrite = fopen( nametoWrite, "w");
  writeFile(toWrite);
  fclose(toWrite);
  return 0;

}

//----------------------------------------
//   Metodos
//----------------------------------------

/*
 * Aparta memoria
 */
void allocate_All(){
  time_t start = time(NULL);
  int i;
  printf("Allocating...\n");
  matrices = malloc(nTh*sizeof(float*));
  refVecs = malloc(nTh*sizeof(float*));
  for( i = 0; i < nTh; i++){
    matrices[i] = malloc(9*sizeof(float));
    refVecs[i] = malloc(3*sizeof(float));
  }
  volumes = malloc(nTh*sizeof(float));
  fstate = malloc(nTot*sizeof(float*));
  for( i = 0; i < nTot; i++){
    fstate[i] = malloc(3*sizeof(float));
  }
  Ids = malloc(nTot*sizeof(unsigned int));
  printf("Time elapsed: %f\n", (float)(time(NULL) - start));
}

/*
 * Lee el archivo de condiciones finales y lo guarda en fstate
 */
void readFile(FILE* data){
  time_t start = time(NULL);
  printf("Reading final state...\n");
  float tmp;
  float x,y,z;
  unsigned int id;
  int test;
  do{
    test = fscanf(data, "%d %f %f %f %f %f %f %f\n", &id, &tmp, &x, &y, &z, &tmp, &tmp, &tmp);
    fstate[id-1][0] = x;
    fstate[id-1][1] = y;
    fstate[id-1][2] = z;
  }while( test!=EOF );
  printf("Last ID: %d\n", id);
  printf("Time elapsed: %f\n", (float)(time(NULL) - start));
}

/*
 * Lee el archivo de puntos y lo guarda points
 */
void readFile2(FILE* data){
  time_t start = time(NULL);
  printf("Reading points...\n");
  float x,y,z;
  // Cuenta cuantos puntos hay para poder apartar memoria
  unsigned int count = 0;
  int test;
  do{
    test = fscanf(data, "%f %f %f\n", &x, &y, &z);
    count ++;
  }while( test!=EOF );
  count--; // Se pasa en 1 debido a la formacion del while
  // Aparta memoria para points y llena el arreglo
  points = malloc(count*sizeof(float*));
  unsigned int i;
  for( i = 0; i < count; i++){
    points[i] = malloc(3*sizeof(float));
  }
  // Aparte memoria para densities y llena el arreglo de ceros
  densities = malloc(count*sizeof(float));
  for( i = 0; i < count; i++){
    densities[i] = 0;
  }
  npoints = count;
  printf("Numero de puntos contados: %d\n",npoints);
  // Rewind para leer otra vez
  rewind(data);
  for(count = 0; count < npoints; count++){
    fscanf(data, "%f %f %f\n", &x, &y, &z);
    points[count][0] = x;
    points[count][1] = y;
    points[count][2] = z;
  }
  printf("Numero de puntos leidos: %d",count);
  printf("Time elapsed: %f\n", (float)(time(NULL) - start));
}

/*
 * Llena el array de matrices y tetraedros
 */
void mkThdrons(){
   time_t start = time(NULL);
   printf("Making tetrahedrons...\n");
   // Longitud de la arista de los cubos
   unsigned int L = 128;
   // Iteradores
   unsigned int l,i,j,k,g;
   g = 0;
   // numero de cubos en la simulacion
   unsigned int nit = nTot/pow(L,3);
   for( l = 0; l < nit; l++ ){
     for( i = 0; i < L-1; i++ ){
       for( j = 0; j < L-1; j++ ){
         for( k = 0; k < L-1; k++ ){
           // Id de las esquina de un tetraedro
           unsigned int point = l*pow(L,3) + i*pow(L,2) + j*L + k;
           unsigned int point2 = point + pow(L,2) + L + 1;
           unsigned int point3, point4;
           // Volumen del tetraedro
           float vol;
           // Vector de referencia (esquina del tetraedro)
           float* rv = fstate[point];
           // Matriz de transformada baricentrica
           float* mv = malloc(9*sizeof(float));
           mv[0] = fstate[point2][0] - fstate[point][0];
           mv[3] = fstate[point2][1] - fstate[point][1];
           mv[6] = fstate[point2][2] - fstate[point][2];
           //------------------------------------------------------------------------------
           // Primer tetraedro
           //------------------------------------------------------------------------------
           point3 = point+L;
           point4 = point+(L*L)+L;
           mv[1] = fstate[point3][0] - fstate[point][0];
           mv[4] = fstate[point3][1] - fstate[point][1];
           mv[7] = fstate[point3][2] - fstate[point][2];
           mv[2] = fstate[point4][0] - fstate[point][0];
           mv[5] = fstate[point4][1] - fstate[point][1];
           mv[8] = fstate[point4][2] - fstate[point][2];
           vol = det(mv)/(2*pow(10,10));
           if( vol != 0){
             refVecs[g] = rv;
             matrices[g] = inverse(mv);
             volumes[g] = vol;
             //printf("La inversa de la matriz es: \n%f  %f  %f\n%f  %f  %f\n%f  %f  %f\n", matrices[g%7][0],  matrices[g%7][1],  matrices[g%7][2],
             //matrices[g%7][3],  matrices[g%7][4],  matrices[g%7][5],  matrices[g%7][6],  matrices[g%7][7],  matrices[g%7][8]);
             add_to_Tree(point, point2, point3, point4, g);
             g++;
           }
           //------------------------------------------------------------------------------
           // Segundo tetraedro
           //------------------------------------------------------------------------------
           point3 = point+L;
           point4 = point+1+L;
           mv[1] = fstate[point3][0] - fstate[point][0];
           mv[4] = fstate[point3][1] - fstate[point][1];
           mv[7] = fstate[point3][2] - fstate[point][2];
           mv[2] = fstate[point4][0] - fstate[point][0];
           mv[5] = fstate[point4][1] - fstate[point][1];
           mv[8] = fstate[point4][2] - fstate[point][2];
           vol = det(mv)/(2*pow(10,10));
           if( vol != 0){
             //printf("%f\n",1/vol);
             refVecs[g] = rv;
             matrices[g] = inverse(mv);
             volumes[g] = vol;
             add_to_Tree(point, point2, point3, point4, g);
             g++;
           }
           //------------------------------------------------------------------------------
           // Tercer tetraedro
           //------------------------------------------------------------------------------
           point3 = point+(L*L);
           point4 = point+(L*L)+L;
           mv[1] = fstate[point3][0] - fstate[point][0];
           mv[4] = fstate[point3][1] - fstate[point][1];
           mv[7] = fstate[point3][2] - fstate[point][2];
           mv[2] = fstate[point4][0] - fstate[point][0];
           mv[5] = fstate[point4][1] - fstate[point][1];
           mv[8] = fstate[point4][2] - fstate[point][2];
           vol = det(mv)/(2*pow(10,10));
           if( vol != 0){
             //printf("%f\n",1/vol);
             refVecs[g] = rv;
             matrices[g] = inverse(mv);
             volumes[g] = vol;
             add_to_Tree(point, point2, point3, point4, g);
             g++;
           }
           //------------------------------------------------------------------------------
           // Cuarto tetraedro
           //------------------------------------------------------------------------------
           point3 = point+(L*L);
           point4 = point+(L*L)+1;
           mv[1] = fstate[point3][0] - fstate[point][0];
           mv[4] = fstate[point3][1] - fstate[point][1];
           mv[7] = fstate[point3][2] - fstate[point][2];
           mv[2] = fstate[point4][0] - fstate[point][0];
           mv[5] = fstate[point4][1] - fstate[point][1];
           mv[8] = fstate[point4][2] - fstate[point][2];
           vol = det(mv)/(2*pow(10,10));
           if( vol != 0){
             //printf("%f\n",1/vol);
             refVecs[g] = rv;
             matrices[g] = inverse(mv);
             volumes[g] = vol;
             add_to_Tree(point, point2, point3, point4, g);
             g++;
           }
           //------------------------------------------------------------------------------
           // Quinto tetraedro
           //------------------------------------------------------------------------------
           point3 = point+1;
           point4 = point+1+L;
           mv[1] = fstate[point3][0] - fstate[point][0];
           mv[4] = fstate[point3][1] - fstate[point][1];
           mv[7] = fstate[point3][2] - fstate[point][2];
           mv[2] = fstate[point4][0] - fstate[point][0];
           mv[5] = fstate[point4][1] - fstate[point][1];
           mv[8] = fstate[point4][2] - fstate[point][2];
           vol = det(mv)/(2*pow(10,10));
           if( vol != 0){
             //printf("%f\n",1/vol);
             refVecs[g] = rv;
             matrices[g] = inverse(mv);
             volumes[g] = vol;
             add_to_Tree(point, point2, point3, point4, g);
             g++;
           }
           //------------------------------------------------------------------------------
           // Sexto tetraedro
           //------------------------------------------------------------------------------
           point3 = point+1;
           point4 = point+(L*L)+1;
           mv[1] = fstate[point3][0] - fstate[point][0];
           mv[4] = fstate[point3][1] - fstate[point][1];
           mv[7] = fstate[point3][2] - fstate[point][2];
           mv[2] = fstate[point4][0] - fstate[point][0];
           mv[5] = fstate[point4][1] - fstate[point][1];
           mv[8] = fstate[point4][2] - fstate[point][2];
           vol = det(mv)/(2*pow(10,10));
           if( vol != 0){
             refVecs[g] = rv;
             matrices[g] = inverse(mv);
             volumes[g] = vol;
             add_to_Tree(point, point2, point3, point4, g);
             g++;
           }
         }
       }
     }
   }
   realnTh = g;
   //for(i = 0; i< g; i++){
     //printf("La inversa de la matriz es: \n%f  %f  %f\n%f  %f  %f\n%f  %f  %f\n", matrices[i][0],  matrices[i][1],  matrices[i][2], matrices[i][3],  matrices[i][4],  matrices[i][5],  matrices[i][6],  matrices[i][7],  matrices[i][8]);
   //}
   printf("Last Tetrahedron: %d\n", g);
   printf("Time elapsed: %f\n", (float)(time(NULL) - start));
 }

/*
 * Llena el arbol con los tetraedros
 * pi : la posicion del punto en fstate
 * la posicion del tetraedro en el array de tetraedros
 */
void add_to_Tree(int p1, int p2, int p3, int p4, int g){
  int j;
  // Marca la mitad de la dimension j en la que se encuentra
  float mid;
  // actualiza la rama en la que esta el punto
  Nodo* actual = Tree;
  for(j = 0; j < 3; j++){
    mid = (rmin[j] + rmax[j])/2;
    // Si esta en un lado, toma una rama, si no, no actualiza la rama

    if((fstate[p1][j] >= mid) && (fstate[p2][j] >= mid) && (fstate[p3][j] >= mid) && (fstate[p4][j] >= mid)){
      actual = actual->right;
    }
    else if((fstate[p1][j] < mid) && (fstate[p2][j] < mid) && (fstate[p3][j] < mid) && (fstate[p4][j] < mid)){
      actual = actual->left;
    }
    else{
      j = 3;
    }
  }
  // Inserta el tetraedro donde haya quedado
  if(actual->thdrons == 0){
    actual->thdrons = iniList(g);
  }
  else{
    add(actual->thdrons, g);
  }
}

/*
 * Samplea las densidades en las posiciones dadas
 */
void getDensities(){
  time_t start = time(NULL);
  printf("Getting Densities...\n");
  // Indices del proceso
  int i,j,k;
  float* temp;
  float* temp2 = malloc(3*sizeof(float));
  // Marca la mitad de la dimension j en la que se encuentra
  float mid;
  // actualiza la rama en la que esta el punto y concatena las listas de tetrahedros que probablemente contengan al punto
  // segun su ubicacion
  for( k = 0; k < npoints; k++){
    printf("%d\n", k);
    List** efect = malloc(4*sizeof(List*));
    efect[0] = Tree->thdrons;
    Nodo* actual = Tree;
    for(j = 0; j < 3; j++){
      mid = (rmin[j] + rmax[j])/2;
      if( points[k][j] >= mid ){
        actual = actual->right;
      }
      else{
        actual = actual->left;
      }
      // Añade la nueva lista al arbol (hace un deep copy)
      efect[j+1] = actual->thdrons;

    }
    for( j = 0; j < 4; j++ ){
      List* ini = efect[j];
      do{
        i = ini->index;
        float vol = volumes[i];
        float* vic = refVecs[i];
        float* matriz = matrices[i];

        temp2[0] = points[k][0] - vic[0];
        temp2[1] = points[k][1] - vic[1];
        temp2[2] = points[k][2] - vic[2];
        temp = product(matriz,temp2);
        if( (temp[0] >= 0 ) && (temp[1] >= 0 ) && (temp[2] >= 0 ) && (temp[0] - 1 <= 0)  && (temp[1] - 1 <= 0) && (temp[2] - 1 <= 0) ){
          densities[k] += fabs(1/vol);
        }
        free(temp);
        ini = ini->next;
      }while(ini != 0);
    }
  }
  printf("Time elapsed: %f\n", (float)(time(NULL) - start));
}

/*
 * Escribe las densidades encontradas
 */
void writeFile(FILE * data){
  time_t start = time(NULL);
  printf("Writing Densities...\n");
  unsigned int i;
  for( i = 0; i < npoints; i++){
    fprintf(data, "%f\n", densities[i]);
  }
  printf("Time elapsed: %f\n", (float)(time(NULL) - start));
}

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//--- Metodos inicializadores del arbol y la lista -----------------------------
//------------------------------------------------------------------------------

/*
 * Inicializa un nodo vacio con los límites dados
 */
Nodo* iniNodo(){
  Nodo *nodo = malloc(sizeof(Nodo));
  nodo->left = 0;
  nodo->right = 0;
  nodo->thdrons = 0;
  return nodo;
}

/*
 * Inicializa una lista vacía dado un índice;
 */
List* iniList( int index ){
  List *li = malloc(sizeof(List));
  li->next = 0;
  li->index = index;
  li->last = li;
  return li;
}

/*
 * Inicializun arbol vacío de orden n
 */
Nodo* iniTree(int orden){
  // Revisar
  Nodo* tree = iniNodo();
  if(orden == 0){
    return tree;
  }
  orden--;
  tree->left = iniTree(orden);
  tree->right = iniTree(orden);
  return tree;
}

/*
 * Concatena lista2 a lista
List** cat(List* lista, List* lista2){
  if(lista != 0 && lista2 != 0){
    // Aumenta el numero de elementos en la lista primera.
    //lista->n = lista->n + lista2->n;
    // Crea el elemento a la lista
    // Añade el nuevo elemento en la cola y cambia la referencia al elemento final en el primer nodo
    //(lista->last)->next = lista2;
    //lista->last = lista2->last;
    do{

    }
  }
  else if(lista == 0 && lista2 != 0){
    lista = lista2;
  }
}
*/

/*
 * Añade un nuevo elemento a la lista
 */
void add(List* lista, unsigned int index){
  // Aumenta el numero de elementos en la lista primera.
  List *li = malloc(sizeof(List));
  li->next = 0;
  //li->n = 1;
  li->index = index;
  li->last = li;
  //lista->n = lista->n +1;
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

//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------
//   Metodos LinAlg
//----------------------------------------
/*
 * Retorna el producto de una matriz 3x3 con un vector 3d
 * m: Matriz compuesta por vectores fila i+3j
 * b: un vector columna
 */
float* product(float* m1, float* b1){
  float *m = m1;
  float *b = b1;
  float* ans = malloc(9*sizeof(float));
  ans[0] = m[0]*b[0] + m[1]*b[1] + m[2]*b[2];
  ans[1] = m[3]*b[0] + m[4]*b[1] + m[5]*b[2];
  ans[2] = m[6]*b[0] + m[7]*b[1] + m[8]*b[2];
  return ans;
}
/*
 * Retorna el producto cruz de dos vectores 3d
 */
float* cross(float* a1,float* b1){
  float *a = a1;
  float *b = b1;
  float* ans = malloc(9*sizeof(float));
  ans[0] = a[1]*b[2] - a[2]*b[1];
  ans[1] = b[0]*a[2] - b[2]*a[0];
  ans[2] = a[0]*b[1] - a[1]*b[0];
  return ans;
}
/*
 * Retorna el determinante de una matriz 3x3
 */
float det(float* m1){
  // Mirar si es efectivo guardar las coordenadas en variables
  float *m = m1;
  return m[0]*(m[4]*m[8] - m[5]*m[7]) + m[1]*(m[6]*m[5] - m[8]*m[3]) + m[2]*(m[3]*m[7] - m[4]*m[6]) ;
}
/*
 * Retorna la inversa de una matriz (asume que se puede)
 *  0   1   2
 *  3   4   5
 *  6   7   8
 */
float* inverse(float* m1){
  float *m = m1;
  float* inv = malloc(9*sizeof(float));
  // Uno sobre el determinante
  float undet = 1.0/det(m);
  inv[0] =  (m[4]*m[8] - m[5]*m[7])*undet;
  inv[1] = -(m[1]*m[8] - m[7]*m[2])*undet;
  inv[2] =  (m[1]*m[5] - m[2]*m[4])*undet;
  inv[3] = -(m[3]*m[8] - m[5]*m[6])*undet;
  inv[4] =  (m[0]*m[8] - m[2]*m[6])*undet;
  inv[5] = -(m[0]*m[5] - m[2]*m[3])*undet;
  inv[6] =  (m[3]*m[7] - m[4]*m[6])*undet;
  inv[7] = -(m[0]*m[7] - m[1]*m[6])*undet;
  inv[8] =  (m[0]*m[4] - m[1]*m[3])*undet;
  return inv;
}
