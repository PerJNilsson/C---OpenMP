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
double sqrt(double x);
double calculate_3d_distance(double point1[], double point2[])


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


  // förstår inte detta /e
  // det räcker med storlek size*(size-1)/2 för distance matrix
  for (size_t i=0, j=0; i<size; i++, j+=3){
    distanceMatrix[i] = distancePointer + j; 
  }


  
  fp_cell = fopen("/home/hpc2018/a3_grading/cell_e5", "r");
  
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
  
  omp_set_dynamic(0);
  nThreads = 20;//atoi(argv[1]+2);

  omp_set_num_threads(nThreads);
  /* Fork a team of threads giving them their own copies of variables */
#pragma omp parallel private(nThreads, tid)
  { 
  #pragma omp for
  for (size_t i = 0; i<size; i++){
    for (size_t j = 0; j<size; j++){
      
      distanceMatrix[i][j] = calculate_3d_distance(coordinateMatrix[i], coordinateMatrix[j]);
            
      }
    }


 }

  
  return 0;
}


double calculate_3d_distance(double point1[], double point2[]){
  double distance;
  distance = sqrt( (point1[0]-point2[0])*(point1[0]-point2[0]) +\
                   (point1[1]-point2[1])*(point1[1]-point2[1]) +\
                   (point1[2]-point2[2])*(point1[2]-point2[2]));
  return distance;
