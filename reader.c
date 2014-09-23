#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void readBinary(char* name);

// Estructura que contiene el header (256 bytes)
struct Header{
  int npart[6];
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
  char fill[256 - 6 * 4 - 6 * 8 - 2 * 8 - 2 * 4 - 6 * 4 - 2 * 4 - 4 * 8];	/* fills to 256 Bytes */
} header;

// Estructura que contiene la información importante de las partículas
struct Particle{
  float Pos[3];
  float Vel[3];
  float Mass;
  int Type;
} *P;

// Crea el contenedor de IDs
int *Id;

int main(){



  return 0;
}

void readBinary( char* name ){
 
  // Abre el archivo para leer en binario
  FILE* file = fopen( name, "rb" );
  if( !file ){
    printf( "Unable to open %s", name );
    fclose(file);
    return;
  }

  // Obtiene el tamaño del archivo
  unsigned long length;
  fseek( file, 0, SEEK_END);
  length = ftell( file );
  fseek( file, 0, SEEK_SET ); 

  // Abre y cierra los bloques 
# define SKIP fread(&dummy, sizeof(dummy), 1, file);

  //-------------------------------------------------------------
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Variables
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //-------------------------------------------------------------

  
  int dummy; // Skip
  int numP = 0; // Numero de particulas
  int i,j,k; // Indices

  //-------------------------------------------------------------
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Header
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //-------------------------------------------------------------


  SKIP; // Abre el bloque
  fread( &header, sizeof(struct Header), 1, file );
  SKIP; // Cierra el bloque

  // Obtiene el numero de particulas
  for( k = 0; k < 6; k++){
    numP += header.npart[k];
  }

  // Aparta memoria
  P = malloc(numP*sizeof(struct Particle));
  Id = malloc(numP*sizeof(int));
  
  //-------------------------------------------------------------
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Reader posiciones-velocidades-masas-ids
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //-------------------------------------------------------------

  // Posiciones
  SKIP;
  for( i = 0; i < numP; i++){
    fread( &P[i].Pos, sizeof(P[i].Pos), 1, file );
  }
  SKIP;

  // Velocidades
  SKIP;
  for( i = 0; i < numP; i++){
    fread( &P[i].Vel, sizeof(P[i].Vel), 1, file );
  }
  SKIP;

  // IDs
  SKIP;
  fread( &Id, sizeof(Id), 1, file );
  SKIP;

  // Masas
  SKIP;
  for( i = 0; i < 6; i++){
    int sum = 0;
    if( header.mass[i] == 0 ){
      for( j = 0; j < header.npart[i]; j++ ){
	fread( &P[sum + j].Mass, sizeof(P[i].Mass), 1, file );
      }
    }
    else{
      for( j = 0; j < header.npart[i]; j++ ){
	P[sum +j].Mass = header.mass[i];
      }
    }
    sum += header.npart[i];
  }
  SKIP;
}

