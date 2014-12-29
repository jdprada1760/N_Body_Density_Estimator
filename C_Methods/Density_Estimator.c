/*
 * Programa que imprime la densidad de una simulacion dada por fstate en un conjunto de puntos dados.
 * Para incluir la libreria de algebra lineal: cc filename.c -o out.x libLinalg.a
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define man "./estimar.x finalState points toWrite"
#define n = pow(128,3);
#define nTh = 6*pow(127,3);

//----------------------------------------
//   Declaracion Metodos
//----------------------------------------
void allocate_All();
void readFile(FILE* data, float* fstate);
// Linalg
float* product(float* m1, float* b1);
float* cross(float* a1,float* b1);
float det(float* m1);
float* inverse(float* m1);



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
  // Crea los tetraedros definidos por matrices, refvecs y volumes


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
  ans[0] = m[0]*b[0] + m[3]*b[1] + m[6]*b[2];
  ans[1] = m[1]*b[0] + m[4]*b[1] + m[7]*b[2];
  ans[2] = m[2]*b[0] + m[5]*b[1] + m[8]*b[2];
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
