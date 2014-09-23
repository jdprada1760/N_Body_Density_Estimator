#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define man "./reader.x toRead toWrite"

void readBinary(char* name);

// Estructura que contiene el header (256 bytes)
struct Header{
  unsigned int npart[6];
  double mass[6];
  double time;
  double redshift;
  int flag_sfr;
  int flag_feedback;
  int npartTotal[6];
  int flag_cooling;
  int num_files;
  double BoxSize;
  double Omega0;
  double OmegaLambda;
  double HubbleParam;
  int flag_stellarAge;
  int flag_metals;
  int numPart_total_hw;
  int flag_entropy_ics;
  char fill[256 - sizeof(unsigned int) - 7*sizeof(double) - 9*sizeof(int)];	/* fills to 256 Bytes */
};

// Estructura que contiene la información importante de las partículas
struct Particle{
  float* Pos;
  float* Vel;
  float Mass;
  int Type;
};

// Crea el contenedor de IDs
int* Id;
int numP = 0; //Numero de Particulas
struct Header header;
struct Particle* P;

int main(int argc, char **argv){
  printf("Begin...\n");
  readBinary( argv[1] );
  FILE *fila = fopen( argv[2], "w" );
  // Imprime un archivo con las posiciones, velocidades, masas y IDs de las partículas
  int i;
  printf("Writing...\n");
  i = 0;
  fprintf( fila, "Archivo\n");

  for( i = 0; i < numP; i++ ){
    printf("%d\n",i);
    fprintf( fila, "%d %f %f %f %f %f %f\n", Id[i], P[i].Pos[0], P[i].Pos[1], P[i].Pos[2], P[i].Vel[0] ,P[i].Vel[1] ,P[i].Vel[2] );
  }
  fclose(fila);
  return 0;
}

void readBinary( char* name ){
  FILE *fila = fopen("phase_Space.dat", "w");
  // Abre el archivo para leer en binario
  printf("Opening file...\n");
  FILE* file = fopen( name, "r" );
  if( !file ){
    printf( "Unable to open %s", name );
    fclose(file);
    return;
  }

  // Abre y cierra los bloques 
# define SKIP fread(&dummy, sizeof(dummy), 1, file)

  //-------------------------------------------------------------
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Variables
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //-------------------------------------------------------------

  
  int dummy; // Skip
  int i,j; // Indices

  //-------------------------------------------------------------
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Header
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //-------------------------------------------------------------

  printf("Geting header...\n");
  SKIP; // Abre el bloque
  fread( &header, sizeof(struct Header), 1, file );
  SKIP; // Cierra el bloque

  // Obtiene el numero de particulas
  printf("Getting number of particles...\n");
  numP += header.npart[5];
  printf("%d\n", header.npart[5]);
  

  // Aparta memoria
  printf("Allocating memory...\n");
  P = malloc(numP*sizeof(struct Particle));
  Id = malloc(numP*sizeof(int));
  for( i = 0; i < numP; i++){
    P[i].Pos = malloc(3*sizeof(float));
    P[i].Vel = malloc(3*sizeof(float));
  }
  printf("%d\n", numP);

  //-------------------------------------------------------------
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Reader posiciones-velocidades-masas-ids
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //-------------------------------------------------------------

  // Posiciones
  printf("Getting Positions...\n");
  SKIP;
  for( i = 0; i < numP; i++){
    for( j = 0; j < 3; j++){
      fread( &P[i].Pos[j], sizeof(float), 1, file );
      fprintf(fila, "%f\n", P[i].Pos[j]);
    }
  }
  SKIP;

  // Velocidades
  printf("Getting Velocities...\n");
  SKIP;
  for( i = 0; i < numP; i++){
    for( j = 0; j < 3; j++){
      fread( &P[i].Vel[j], sizeof(float), 1, file );
    }
  }
  SKIP;

  // IDs
  printf("Getting Particle IDs...\n");
  SKIP;
  fread( &Id, sizeof(Id), 1, file );
  SKIP;

  // Masas
  printf("Getting Masses...\n");
  SKIP;
  int sum = 0;
  i = 5;
  if( header.mass[i] == 0 ){
    for( j = 0; j < header.npart[i]; j++ ){
      fread( &P[sum].Mass, sizeof(float), 1, file );
      sum++;
    }
  }
  else{
    for( j = 0; j < header.npart[i]; j++ ){
      P[sum].Mass = header.mass[i];
    }
  }
  sum++;
  SKIP;
  printf("End...\n");
  fclose(file);
  fclose(fila);
}

