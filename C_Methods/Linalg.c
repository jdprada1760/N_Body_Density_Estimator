/*
 * Libreria de algebra lineal 3d
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------
//   Metodos
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
 * Retorna el producto punto entre los vectores a y b
 */
float dot(float* a1, float* b1){
  float *a = a1;
  float *b = b1;
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
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

float* by(float* m, float* m0){
  float *m1 = m;
  float *m2 = m0;
  float* ans = malloc(9*sizeof(float));
  // Uno sobre el determinante
  int i;
  int j;
  for( i = 3; i != 0; i--){
    for( j = 3; j != 0; j--){
      ans[3*(i-1) + j-1] = m1[3*(i-1)]*m2[j-1] + m1[3*(i-1)+1]*m2[3+j-1] + m1[3*(i-1)+2]*m2[6+j-1];
    }
  }
  return ans;
}
