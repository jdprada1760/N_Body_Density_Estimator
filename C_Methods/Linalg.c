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

int main(){
  return 0;
}

//----------------------------------------
//   Metodos
//----------------------------------------
/*
 * Retorna el producto de una matriz 3x3 con un vector 3d
 * m: Matriz compuesta por vectores fila
 * b: un vector columna
 */
float* product(float* m, float* b){
  float* ans = malloc(9*sizeof(float));
  ans[0] = m[0]*b[0] + m[3]*b[1] + m[6]*b[2];
  ans[1] = m[1]*b[0] + m[4]*b[1] + m[7]*b[2];
  ans[2] = m[2]*b[0] + m[5]*b[1] + m[8]*b[2];
  return ans;
}
/*
 * Retorna el producto cruz de dos vectores 3d
 */
float* cross(float* a,float* b){
  float* ans = malloc(9*sizeof(float));
  ans[0] = a[1]*b[2] - a[2]*b[1];
  ans[1] = b[0]*a[2] - b[2]*a[0];
  ans[2] = a[0]*b[1] - a[1]*b[0];
  return ans;
}
/*
 * Retorna el determinante de una matriz 3x3
 */
float det(float* m){

}
/*
 * Retorna la inversa de una matriz, null si no tiene inversa
 */
float* inv(flaot* m){

}
