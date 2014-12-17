/*
 * Programa que imprime la densidad de una simulacion dada por fstate en un conjunto de puntos dados.
 * Para incluir la libreria de algebra lineal: cc filename.c -o out.x libLinalg.a
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define man "./estimar.x finalState points toWrite"
#define n = pow(128,3)
#define nTh = 6*pow(8,3)*pow(15,3)

//----------------------------------------
//   Declaracion Metodos
//----------------------------------------
void allocate_All();
void readFile(FILE* data, float* fstate);

int main(int argc, char **argv){
  //----------------------------------------
  //   Variables generales
  //----------------------------------------

  // Los valores que definen un tetraedro
  float* matrices;
  float* refVecs;
  float* volumes;
  // El estado final de la simulacion
  float* fstate;
  unsigned int* Ids;

  // EL archivo donde se guardaron las condiciones finales
  FILE *data;
  char *name = argv[1];
  // Aparta memoria
  allocateAll();
  // El archivo de texto de condiciones iniciales y de evolucion en el tiempo
  data = fopen( name, "r" );
  // Lee el archivo data a fstate
  readFile(data, fstate);


}

//----------------------------------------
//   Metodos
//----------------------------------------

/*
 * Aparta memoria
 */
void allocate_All(){
   int i,j,k;
   matrices = malloc(3*3*nTh*sizeof(float**));
   refvecs = malloc(3*nTh*sizeof(float*));
   volumes = malloc(nTh*sizeof(float));
   fstate = malloc(3*n*sizeof(float*));
   Ids = malloc(n*sizeof(unsigned int));
}

void readFile(FILE* data, float* fstate){
  /*
   * Lee el archivo y lo guarda en fstate
   */
   float tmp;
   float x,y,z;
   unsigned int id;
   do{
     int test = fscanf(data, "%d %f %f %f %f %f %f %f\n", &id, &tmp, &x, &y, &z, &tmp1, &tmp1, &tmp1);
     fstate[3*id+0] = x;
     fstate[3*id+1] = y:
     fstate[3*id+2] = z;
     }while( test!=EOF );
}
