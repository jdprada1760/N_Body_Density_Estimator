/*
 * Libreria de algebra lineal 3d
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------
//   Declaracion Metodos
//----------------------------------------
float* product(float* m, float* b);
float* cross(float* a,float* b);
float det(float* m);
float* inverse(float* m);
float dot(float* a, float* b);
float* by(float* m, float* m0);

int main(){
  float *matrix = malloc(9*sizeof(float));
  matrix[0] = -5;
  matrix[1] = 3;
  matrix[2] = 1;
  matrix[3] = 6;
  matrix[4] = -7;
  matrix[5] = 8;
  matrix[6] = 2;
  matrix[7] = 8;
  matrix[8] = 9;
  printf("El determinante de la matriz es: %f\n", det(matrix));
  float *inv = inverse(matrix);
  printf("La inversa de la matriz es: \n%f  %f  %f\n%f  %f  %f\n%f  %f  %f\n", inv[0], inv[1], inv[2], inv[3], inv[4], inv[5], inv[6], inv[7], inv[8]);
  inv = by(matrix,inv);
  printf("El producto de la matriz por su inversa es : \n%f  %f  %f\n%f  %f  %f\n%f  %f  %f\n", inv[0], inv[1], inv[2], inv[3], inv[4], inv[5], inv[6], inv[7], inv[8]);

  return 0;
}

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
  float undet = 1/det(m);
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
