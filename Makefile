.PHONY: all

all:	cell_distance


cell_distance:	cell_distance.c
	gcc -o cell_distance cell_distance.c -march=native -O3 -fopenmp -lm


clean:
	rm cell_distance

