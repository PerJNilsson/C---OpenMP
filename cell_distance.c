#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>
// Define global variables
FILE *fp_cell;
double ** coordinateMatrix;
double * cellPointer;
int * distanceArray;
// Initialize funtions
double sqrt(double x);

int main(int argc, char *argv[]){

  char file_name[] = "cells";
  //printf("\nFile name is: %s\n\n", file_name);

  fp_cell = fopen(file_name, "r");
  fseek(fp_cell,0, SEEK_END);
  unsigned long long size = ftell(fp_cell)/24;
  fseek(fp_cell, 0, SEEK_SET);
  fclose(fp_cell);
  
  short maximumLengthResolution = 3466;

  double (*coordinateMatrix)[3] = malloc(sizeof(double)*size*3);
  /*
  cellPointer = malloc(sizeof(double) *3*size);
  coordinateMatrix = malloc(sizeof(double) *size);
  for (size_t i=0, j=0; i<size; i++, j+=3){
    coordinateMatrix[i] = cellPointer + j; 
    }*/
 
  // Each distance with 2 decimals can be saved as a short by multiplying the number with 100..
  // For each distance the corresponding elemten in distanceMatrix will +=1
  // Using sizeof(unsigned long long) if it happens that all are at the same point and we have 2^32 coordinates.
  distanceArray =calloc(maximumLengthResolution,sizeof(unsigned long long));

   // Initialize the number of threads, where #threads will be the interger after -t in argv[1], else it exits the program.
  int nThreads;
  nThreads = atoi(argv[1]+2); if (nThreads == 0 || nThreads> 112) {printf("Wrong argument please write -t#, 0<#<112 being an integer."); exit(-1);}
  omp_set_num_threads(nThreads);


  // Unparallelized reading of a file. 
  fp_cell = fopen(file_name, "r");
  for (int i = 0; i < size; i++) {
      double n,m,l;
      // fscanf checks each row which is the coordinate for each point
      fscanf(fp_cell, "%lf %lf %lf", &n, &m, &l);
      coordinateMatrix[i][0] = n;
      coordinateMatrix[i][1] = m;
      coordinateMatrix[i][2] = l;  
    }
  fseek(fp_cell, 0, SEEK_SET);
  fclose(fp_cell);  
  
  /* Initializing parallel threading with reduction meaning the shared array distanceArray will have all its elements from each thread summed up after all threads are finished with their computations.
The dynamic scheduling will divide the payload for each thread equally, which is necessary since our first row barely have any comupations and our last rows have a lot of computations. */
#pragma omp parallel for schedule(dynamic,1) reduction(+:distanceArray[:maximumLengthResolution])
  for (int i = 0; i<size; i++){
    for (int j = 0; j<i; j++){
      int tmpShortDistance =0;
      tmpShortDistance =0.5+100*sqrt(((coordinateMatrix[i][0]-coordinateMatrix[j][0]) * (coordinateMatrix[i][0]-coordinateMatrix[j][0]))+ \
		       ((coordinateMatrix[i][1]-coordinateMatrix[j][1]) * (coordinateMatrix[i][1]-coordinateMatrix[j][1]))+\
		       ((coordinateMatrix[i][2]-coordinateMatrix[j][2]) * (coordinateMatrix[i][2]-coordinateMatrix[j][2])));
      
      {
	distanceArray[tmpShortDistance] +=1;

      }
    }
  }
  unsigned long long sum = 0;
  
  for (int i = 0; i<maximumLengthResolution; i++) {
    if (distanceArray[i] != 0 && i < 1000) {
      printf("%.2f  %d\n", i/100.0, distanceArray[i]);
    }
    else if (distanceArray[i] !=0) {
      printf("%.2f %d\n", i/100.0, distanceArray[i]);
    }
  }
  
  return 0;
}
