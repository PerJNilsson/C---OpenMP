#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <float.h>
// Define global variables
FILE *fp_cell;
double ** coordinateMatrix;
double * cellPointer;
long long * distanceArray;
// Initialize funtions
double sqrt(double x);


int main(int argc, char *argv[]){

  // size = hardcoded number since we know how many elements it is in the file
  char file_name[] = "cell_e5";
  int size = 10000;
  
  short maximumLengthResolution = 3466;
  cellPointer = malloc(sizeof(double) *3*size);
  coordinateMatrix = malloc(sizeof(double) *size);
  for (size_t i=0, j=0; i<size; i++, j+=3){
    coordinateMatrix[i] = cellPointer + j; 
  }
 
  // Each distance with 2 decimals can be saved as a short by multiplying the number with 100..
  // For each distance the corresponding elemten in distanceMatrix will +=1 
  distanceArray =malloc(sizeof(long) *maximumLengthResolution);
  for (size_t i = 0; i<maximumLengthResolution; i++){
    distanceArray[i] = 0;
  }

   // Initialize  the number of threads and the thread ID.
  int nThreads, tid;

  nThreads = atoi(argv[1]+2);
  omp_set_num_threads(nThreads);
  printf("Number of threads: %d\n", nThreads);
 

  
  fp_cell = fopen(file_name, "r");
  /*
#pragma omp parallel private(nThreads, tid)
  {
    tid = omp_get_thread_num();
    #pragma omp for*/
    for (int i = 0; i < size; i++) {
      double n,m,l;
      // Can't make double loop since fscanf only check each row.
      fscanf(fp_cell, "%lf %lf %lf", &n, &m, &l);
      coordinateMatrix[i][0] = n;
      coordinateMatrix[i][1] = m;
      coordinateMatrix[i][2] = l;  
    }
    //}
  fclose(fp_cell);  

  /* Fork a team of threads giving them their own copies of variables */
     //tid = omp_get_thread_num();
  //#pragma omp for
#pragma omp parallel for reduction(+:distanceArray[:maximumLengthResolution])
  for (int i = 0; i<size; i++){
    for (int j = 0; j<size, j!=i; j++){
      int tmpShortDistance =0;
      tmpShortDistance =100*sqrt(((coordinateMatrix[i][0]-coordinateMatrix[j][0]) * (coordinateMatrix[i][0]-coordinateMatrix[j][0]))+ \
		       ((coordinateMatrix[i][1]-coordinateMatrix[j][1]) * (coordinateMatrix[i][1]-coordinateMatrix[j][1]))+\
		       ((coordinateMatrix[i][2]-coordinateMatrix[j][2]) * (coordinateMatrix[i][2]-coordinateMatrix[j][2])));
      //#pragma omp critical (function_critical_section)
      
      //counter+=1;
      {
	distanceArray[tmpShortDistance] +=1;

      }
    }
  } /* All threads join master thread and disband */

  long long sum = 0;
  
  for (int i = 0; i<maximumLengthResolution; i++) {
     if (distanceArray[i] != 0) {
      printf("%.2f  %d\n", i/100.0, distanceArray[i]);
      }
    sum = sum +distanceArray[i];
  }
  
  printf("The sum:%lld\n\n", sum);
  return 0;
}


// Funcitons
