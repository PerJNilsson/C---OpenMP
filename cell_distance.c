#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <string.h>

// Define global variables
FILE *fp_cell;
double ** distanceMatrix;
double ** coordinateMatrix;
double * cellPointer;
// Initialize funtions
void *cell_thread(void *arg);



int main(int argc, char *argv[]){
  int size = 50;
  cellPointer = malloc(sizeof(double) *3*size);
  coordinateMatrix = malloc(sizeof(double) *size);

  for (size_t i=0, j=0; i<size; i++, j+=3){
    coordinateMatrix[i] = cellPointer + j; 
  }

  fp_cell = fopen("cell_50", "r");
  
  for (int i = 0; i < size; i++) {
    //for (int j = 0; j < 3; j++) {
      double n,m,l;
      fscanf(fp_cell, "%lf %lf %lf", &n, &m, &l);
      //printf("(%d, %d) = %lf\n", i, j, n);
      coordinateMatrix[i][0] = n;
      coordinateMatrix[i][1] = m;
      coordinateMatrix[i][2] = l;
      
  }
  fclose(fp_cell);


  return 0;
}


// Creating the function for the pthreads
void *void_thread(void *arg){
  struct arg_pointer *arg_s = (struct arg_pointer *)arg;
  pthread_exit(0);
}
