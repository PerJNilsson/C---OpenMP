#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <omp.h>

// Define global variables
FILE *fp_cell;
double ** distanceMatrix;
double ** coordinateMatrix;
double * cellPointer;
double * distancePointer;
// Initialize funtions
void *cell_thread(void *arg);
double sqrt(double x);


int main(int argc, char *argv[]){

  // size = hardcoded number since i know how many elements it is in cell_50-file
  int size = 100000;
  cellPointer = malloc(sizeof(double) *3*size);
  coordinateMatrix = malloc(sizeof(double) *size);
  for (size_t i=0, j=0; i<size; i++, j+=3){
    coordinateMatrix[i] = cellPointer + j; 
  }
  distancePointer = malloc(sizeof(double) *size*size);
  distanceMatrix = malloc(sizeof(double) *size);

  for (size_t i=0, j=0; i<size; i++, j+=3){
    distanceMatrix[i] = distancePointer + j; 
  }


  
  fp_cell = fopen("cell_e5", "r");
  
  for (int i = 0; i < size; i++) {
    double n,m,l;
    // Can't make double loop since fscanf only check each row.
    fscanf(fp_cell, "%lf %lf %lf", &n, &m, &l);
    coordinateMatrix[i][0] = n;
    coordinateMatrix[i][1] = m;
    coordinateMatrix[i][2] = l;
      
  }
  fclose(fp_cell);  

  // Initialize  the number of threads and the thread ID.
  int nThreads, tid;

  nThreads = 20;//atoi(argv[1]+2);
  omp_set_num_threads(nThreads);
  /* Fork a team of threads giving them their own copies of variables */
#pragma omp parallel private(nThreads, tid)
  { 
  tid = omp_get_thread_num();
  #pragma omp for
  for (size_t i = 0; i<size; i++){
    for (size_t j = 0; j<size; j++){
      double a,x,y,z;
      distanceMatrix[i][j] = sqrt(((coordinateMatrix[i][0]-coordinateMatrix[j][0]) * (coordinateMatrix[i][0]-coordinateMatrix[j][0]))+ \
				    ((coordinateMatrix[i][1]-coordinateMatrix[j][1]) * (coordinateMatrix[i][1]-coordinateMatrix[j][1]))+\
				    ((coordinateMatrix[i][2]-coordinateMatrix[j][2]) * (coordinateMatrix[i][2]-coordinateMatrix[j][2])));

      }
    }

    	/* Obtain thread number */
  //printf("Hej där hälsningar tråd = %d\n", tid);

  /* Only master thread does this */
  if (tid == 0) 
    {
    nThreads = omp_get_num_threads();
    printf("Bara mastertråden kan skriva detta = %d\n", nThreads);
    }

 }  /* All threads join master thread and disband */

  
  return 0;
}


// Creating the function for the pthreads
void *void_thread(void *arg){
  struct arg_pointer *arg_s = (struct arg_pointer *)arg;
  pthread_exit(0);
}
