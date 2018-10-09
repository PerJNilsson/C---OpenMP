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

// Initialize funtions
void *cell_thread(void *arg);



int main(int argc, char *argv[]){

  // size = hardcoded number since i know how many elements it is in cell_50-file
  int size = 50;
  cellPointer = malloc(sizeof(double) *3*size);
  coordinateMatrix = malloc(sizeof(double) *size);

  for (size_t i=0, j=0; i<size; i++, j+=3){
    coordinateMatrix[i] = cellPointer + j; 
  }

  fp_cell = fopen("cell_50", "r");
  
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

  nThreads = 10;//atoi(argv[1]+2);
  omp_set_num_threads(nThreads);
  /* Fork a team of threads giving them their own copies of variables */
#pragma omp parallel private(nThreads, tid)
  {

  /* Obtain thread number */
  tid = omp_get_thread_num();
  printf("Hej där hälsningar tråd = %d\n", tid);

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
