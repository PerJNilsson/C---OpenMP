#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <string.h>
#include <omp.h>


FILE * fp_cell;
double ** coordinateMatrix;
double * cellPointer;

int count_lines();

int main(int argc, char *argv[]){

  int size;


  cellPointer = malloc(sizeof(double) *3*size);
  coordinateMatrix = malloc(sizeof(double) *size);
  for (size_t i=0, j=0; i<size; i++, j+=3){
    coordinateMatrix[i] = cellPointer + j; 
  }

  fp_cell = fopen("/home/hpc2018/a3_grading/test_data/cell_e5", "r");

  size = count_lines();


  for (int i = 0; i < size; i++) {
    double n,m,l;
    // Can't make double loop since fscanf only check each row.
    fscanf(fp_cell, "%lf %lf %lf", &n, &m, &l);
    coordinateMatrix[i][0] = n;
    coordinateMatrix[i][1] = m;
    coordinateMatrix[i][2] = l;
  }

  fclose(fp_cell);
  
  printf("%d", size);


  return 0;
}




int count_lines(){

  int size=0;
  char ch;
  
  do{
    ch = fgetc(fp_cell);
    if(ch == '\n'){
      size++;
    }
  } while (ch != EOF);

  return size;
    
}


  
