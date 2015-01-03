/*
 * Programa que imprime la densidad de una simulacion dada por fstate en un conjunto de puntos dados.
 * Para incluir la libreria de algebra lineal: cc filename.c -o out.x libLinalg.a
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define man "./estimar.x finalState points toWrite"
#define n pow(128,3)
#define nTh 6*pow(127,3)

//----------------------------------------
//   Declaracion Metodos
//----------------------------------------
void allocate_All();
void readFile(FILE* data);
// Linalg
float* product(float* m1, float* b1);
float* cross(float* a1,float* b1);
float det(float* m1);
float* inverse(float* m1);

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



int main(int argc, char **argv){
  // EL archivo donde se guardaron las condiciones finales
  FILE *data;
  char *name = argv[1];
  // Aparta memoria
  allocate_All();
  // El archivo de texto de condiciones iniciales y de evolucion en el tiempo
  data = fopen( name, "r" );
  // Lee el archivo data a fstate
  readFile(data);
  // Crea los tetraedros definidos por matrices, refvecs y volumes


  return 0;
}

//----------------------------------------
//   Metodos
//----------------------------------------

/*
 * Aparta memoria
 */
void allocate_All(){
  int i;
  matrices = malloc(nTh*sizeof(float*));
  refVecs = malloc(nTh*sizeof(float*));
  for( i = 0; i < nTh; i++){
    matrices[i] = malloc(9*sizeof(float));
    refVecs[i] = malloc(3*sizeof(float));
  }
  volumes = malloc(nTh*sizeof(float));
  fstate = malloc(n*sizeof(float*));
  for( i = 0; i < n; i++){
    fstate[i] = malloc(3*sizeof(float));
  }
  Ids = malloc(n*sizeof(unsigned int));
}

/*
 * Lee el archivo y lo guarda en fstate
 */
void readFile(FILE* data){
   float tmp;
   float x,y,z;
   unsigned int id;
   int test;
   do{
     test = fscanf(data, "%d %f %f %f %f %f %f %f\n", &id, &tmp, &x, &y, &z, &tmp, &tmp, &tmp);
     fstate[id][0] = x;
     fstate[id][1] = y;
     fstate[id][2] = z;
     }while( test!=EOF );
}

/*
 * Llena el array de matrices y tetraedros
 */
 void mkThdrons(){
   // Longitud de la arista de los cubos
   unsigned int L = 16;
   // Iteradores
   unsigned int l,i,j,k,g;
   g = 0;
   // numero de cubos en la simulacion
   unsigned int nit = n/pow(L,3);
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
           vol = det(mv)/2;
           if( vol != 0){
             refVecs[g] = rv;
             matrices[g] = inverse(mv);
             volumes[g] = vol;
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
           vol = det(mv)/2;
           if( vol != 0){
             refVecs[g] = rv;
             matrices[g] = inverse(mv);
             volumes[g] = vol;
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
           vol = det(mv)/2;
           if( vol != 0){
             refVecs[g] = rv;
             matrices[g] = inverse(mv);
             volumes[g] = vol;
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
           vol = det(mv)/2;
           if( vol != 0){
             refVecs[g] = rv;
             matrices[g] = inverse(mv);
             volumes[g] = vol;
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
           vol = det(mv)/2;
           if( vol != 0){
             refVecs[g] = rv;
             matrices[g] = inverse(mv);
             volumes[g] = vol;
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
           vol = det(mv)/2;
           if( vol != 0){
             refVecs[g] = rv;
             matrices[g] = inverse(mv);
             volumes[g] = vol;
             g++;
           }
         }
       }
     }
   }
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
