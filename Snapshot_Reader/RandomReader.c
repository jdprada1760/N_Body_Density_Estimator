#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define man "./reader.x toRead"

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
  char fill[256 - 6*sizeof(unsigned int) - 12*sizeof(double)- 14*sizeof(int)];	/* fills to 256 Bytes */
};

// Estructura que contiene la información importante de las partículas
struct Particle{
  float* Pos;
  float* Vel;
  float Mass;
  int Id;
};

// Crea el contenedor de IDs
int numP = 0; //Numero de Particulas
struct Header header;
struct Particle* P;

int main(int argc, char **argv){
  printf("Begin...\n");
  readBinary( argv[1] );
  return 0;
}

void readBinary( char* name ){
  // Abre el archivo para leer en binario
  printf("Opening file...\n");
  FILE* file = fopen( name, "r" );
  if( !file ){
    printf( "Unable to open %s", name );
    fclose(file);
    return;
  }

  // Abre y cierra los bloques 
# define SKIP fread(&dummy, sizeof(dummy), 1, file); printf("%d\n", dummy)

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
  int type = 1;
  numP += header.npart[type];
  
  /*
  // Determina el tipo de particulas que se quiere
  int k;
  for( k = 0; k < 6; k++){
    printf("%d\n", header.npart[k]);
  }
  */

  // Aparta memoria
  printf("El numero de particulas es %d\n", numP);
  P = malloc(sizeof(struct Particle));

  //-------------------------------------------------------------
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Reader posiciones-velocidades-masas-ids
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //-------------------------------------------------------------

  // Posiciones
  printf("Getting-Writting positions...\n");
  FILE *fila = fopen( "pos", "w" );
  SKIP;
  for( i = 0; i < numP; i++){
    for( j = 0; j < 3; j++){
      fread( &P[0].Pos[j], sizeof(float), 1, file );
    }
    fprintf( fila, "%f %f %f\n", P[0].Pos[0], P[0].Pos[1], P[0].Pos[2] );
  }
  SKIP;
  fclose(fila);

  // Velocidades
  printf("Getting-Writting Velocities...\n");
  fila = fopen( "vel", "w" );
  SKIP;
  for( i = 0; i < numP; i++){
    for( j = 0; j < 3; j++){
      fread( &P[0].Vel[j], sizeof(float), 1, file );
    }
    fprintf( fila, "%f %f %f\n", P[0].Vel[0] ,P[0].Vel[1] ,P[0].Vel[2] );      
  }
  SKIP;
  fclose(fila);

  // IDs
  printf("Getting-Writting Particle IDs...\n");
  fila = fopen( "id", "w" );
  SKIP;
  for( i = 0 ; i < numP; i++ ){
    fread( &P[0].Id, sizeof(int), 1, file );
    fprintf( fila, "%d\n", P[0].Id );      
  }
  SKIP;
  fclose(fila);

  // Masas
  printf("Getting-Writting Masses...\n");
  fila = fopen( "mass", "w" );
  SKIP;
  i = 5;
  if( header.mass[i] == 0 ){
    //printf(" Las masas son iguales\n ");
    for( j = 0; j < header.npart[i]; j++ ){
      fread( &P[0].Mass, sizeof(float), 1, file );
      fprintf( fila, "%f\n", P[0].Mass );     
    }
  }
  else{
    //printf("Las masas son diferentes ");
    for( j = 0; j < header.npart[i]; j++ ){
      P[0].Mass = header.mass[i];
      fprintf( fila, "%f\n", P[0].Mass );     
    }
  }
  SKIP;
  fclose(fila);
  printf("End...\n");
  fclose(file);
}

