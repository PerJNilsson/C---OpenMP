#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <float.h>
// Define global variables
FILE *fp_cell;
FILE * fp_cell_init;
double ** coordinateMatrix;
double * cellPointer;
unsigned long long * distanceArray;
// Initialize funtions
double sqrt(double x);
long count_lines();

int main(int argc, char *argv[]){

  char file_name[] = "cells";
  printf("\nFile name is: %s\n\n", file_name);
  fp_cell = fopen(file_name, "r");
  long size = count_lines();
  fclose(fp_cell);
  
  //long size = 100000;
  
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
  int nThreads;
  
  nThreads = atoi(argv[1]+2); if (nThreads == 0 || nThreads> 112) {printf("Wrong argument please write -t#, # being a number"); exit(-1);}
  omp_set_num_threads(nThreads);
  
  fp_cell = fopen(file_name, "r");
  for (int i = 0; i < size; i++) {
      double n,m,l;
      // fscanf checks each row.
      fscanf(fp_cell, "%lf %lf %lf", &n, &m, &l);
      coordinateMatrix[i][0] = n;
      coordinateMatrix[i][1] = m;
      coordinateMatrix[i][2] = l;  
    }
  fclose(fp_cell);  
  
  // Initializing parallel threading with reduction as stated above
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
      printf("0%.2f  %d\n", i/100.0, distanceArray[i]);
    }
    else if (distanceArray[i] !=0) {
      printf("%.2f %d\n", i/100.0, distanceArray[i]);
    }
    }
  
  return 0;
}

long count_lines(){

  long tmpSize=0;
  char ch;
  do{
    ch = fgetc(fp_cell);
    if(ch == '\n'){
      tmpSize++;
    }
  } while (ch != EOF);
  return tmpSize;    
}
