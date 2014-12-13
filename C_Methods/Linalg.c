/*
 * Libreria de algebra lineal 3d
 */
#include <math.h>

//----------------------------------------
//   Declaracion Metodos
//----------------------------------------
float* product(float* m, float* b);
float* cross(float* a,float* b);
float det(float* m);
float* inv(float* m);
float dot(float* a, float* b);

int main(){
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
  m = m1;
  b = b1;
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
  a = a1;
  b = b1;
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
  m = m1;
  return m[0]*(m[4]*m[8] - m[5]*m[7]) + m[1]*(m[6]*m[5] - m[8]*m[3]) + m[2]*(m[3]*m[7] - m[4]*m[6]) ;
}
/*
 * Retorna el producto punto entre los vectores a y b
 */
float dot(float* a1, float* b1){
  a = a1;
  b = b1;
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}
/*
 * Retorna la inversa de una matriz (asume que se puede)
 */
float* inv(float* m1){
  m = m1;
  float* inv = malloc(9*sizeof(float));
  // Uno sobre el determinante
  undet = 1/det(m);
  inv[0] = 
}
