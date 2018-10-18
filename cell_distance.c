#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


FILE * fp_cell;

double (*block0)[3];
double (*block1)[3];


unsigned long long * distance_arr;

int block_size;
unsigned long long file_size;
int number_of_blocks;
size_t max_index;
int number_of_threads;

int chars_per_line = 24;

int count_lines();
void print_results();
double calculate_distance(double * p0, double * p1);
void calculate_internal(int block_choice, int block_number);
void calculate_pairwise(int block_nr0, int block_nr1);
void read_block(int block_choice, int block_number);
void use_single_block();
void use_multiple_blocks();
void read_full_file();

int main(int argc, char *argv[]){

  if (argc > 2){
    printf("Wrong arguments, please write  -t#, # being number of threads");
    exit(-1);
  }

  number_of_threads = atoi(argv[1]+2);

  if (number_of_threads == 0 || number_of_threads> 112){
    printf("Wrong argument please write -t#, 0<#<112 being an integer.");
    exit(-1);
  }
  
  omp_set_num_threads(number_of_threads);

  fp_cell = fopen("cells", "r");

  file_size = count_lines();

  max_index = 100*sqrt(3*20*20)+5; //5 for safety
  distance_arr = calloc(max_index, sizeof(unsigned long long));

  int max_bytes = 9e8;
  
  block_size = max_bytes/(2*3*sizeof(double));

  number_of_blocks = file_size/block_size+1;

    
  if(number_of_blocks<=2){
    use_single_block();
  } else {
    use_multiple_blocks();
  }

  fclose(fp_cell);

  print_results();

  free(block0);
  free(block1);
  free(distance_arr);

  return 0;

}


void use_multiple_blocks(){


  block0 = malloc(sizeof(double) *3*block_size);
  block1 = malloc(sizeof(double) *3*block_size);

  for(size_t i = 0; i<number_of_blocks; ++i){
    read_block(0,i);
    calculate_internal(0,i);
    for(size_t j = i+1; j<number_of_blocks; ++j){
      read_block(1,j);
      calculate_pairwise(i,j);
    }      
  }

  calculate_internal(1,number_of_blocks-1);

}

void use_single_block(){


  block0 = malloc(sizeof(double) *3*file_size);
  read_full_file();

  size_t distance_index;


#pragma omp parallel for schedule(dynamic) private(distance_index) reduction(+:distance_arr[:max_index])
  for(size_t i = 0; i<file_size; ++i){
    for(size_t j = i+1; j<file_size; ++j){
      distance_index = 0.5+100*calculate_distance(block0[i], block0[j]);      
      distance_arr[distance_index] += 1;      
    }
  }


}



int count_lines(){

  unsigned long long len=0;

  fseek(fp_cell, 0, SEEK_END);
  len = ftell(fp_cell);
  fseek(fp_cell, 0, SEEK_SET);
  return len/chars_per_line;
    
}

void read_full_file(){

  double n,m,l;  

  fseek(fp_cell, 0, SEEK_SET);
  
  for (size_t i = 0; i < file_size; i++) {

    fscanf(fp_cell, "%lf %lf %lf", &n, &m, &l);
    block0[i][0] = n;
    block0[i][1] = m;
    block0[i][2] = l;
  }

}


void read_block(int block_choice, int block_number){

  double (*block)[3];

  switch(block_choice){
  case 0:
    block = block0;
    break;
  case 1:
    block = block1;
    break;
  default:
    printf("ILLEGAL BLOCK CHOICE");
    return;
  }

  size_t start_file_position = block_number*block_size;
  size_t end_index;
  
  if(block_number==number_of_blocks-1){
    end_index = file_size-(number_of_blocks-1)*block_size;
  } else {
    end_index = block_size;
  }
  
  double n,m,l;  

  size_t current_position = ftell(fp_cell);
  long move_number = start_file_position*chars_per_line - current_position;
  
  fseek(fp_cell, move_number, SEEK_CUR);
  
  for (size_t i = 0; i < end_index; i++) {

    fscanf(fp_cell, "%lf %lf %lf", &n, &m, &l);
    block[i][0] = n;
    block[i][1] = m;
    block[i][2] = l;
  }

}

void calculate_internal(int block_choice, int block_number){

  double (*block)[3];

  switch(block_choice){
  case 0:
    block = block0;
    break;
  case 1:
    block = block1;
    break;
  default:
    printf("ILLEGAL BLOCK CHOICE");
    return;
  }

  size_t end_index;
  
  if(block_number==number_of_blocks-1){
    end_index = file_size-(number_of_blocks-1)*block_size;
  } else {
    end_index = block_size;
  }  


  size_t distance_index;
  
#pragma omp parallel for schedule(dynamic) private(distance_index) reduction(+:distance_arr[:max_index])
  for(size_t i = 0; i<end_index; ++i){
    for(size_t j = i+1; j<end_index; ++j){
      distance_index = 0.5+100*calculate_distance(block[i], block[j]);
      distance_arr[distance_index] += 1;
    }
  }

}


void calculate_pairwise(int block_nr0, int block_nr1){

  size_t end_index;
  
  if(block_nr1==number_of_blocks-1){
    end_index = file_size-(number_of_blocks-1)*block_size;
  } else {
    end_index = block_size;
  }

  size_t distance_index;

#pragma omp parallel for private(distance_index) reduction(+:distance_arr[:max_index])
  for(size_t i=0; i<block_size; ++i){
    for(size_t j = 0; j<end_index; ++j){
      distance_index = 0.5+100*calculate_distance(block0[i], block1[j]);
      distance_arr[distance_index] += 1;
    }
  }

}


double calculate_distance(double * p0, double * p1){
  double distance;

  distance = sqrt( (p0[0]-p1[0])*(p0[0]-p1[0]) + \
                   (p0[1]-p1[1])*(p0[1]-p1[1]) + \
                   (p0[2]-p1[2])*(p0[2]-p1[2]) );

  return distance;
}

void print_results(){

  for(size_t i = 0; i<max_index; ++i){
    if(distance_arr[i] > 0){
      printf("%05.2f %d\n", i/100.0, distance_arr[i]);
    }
  }
  
}
