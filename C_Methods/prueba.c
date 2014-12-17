/*
 * Programa para probar la libreria de algebralineal
 * Para incluir la libreria de algebra lineal: cc filename.c -o out.x libLinalg.a
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
