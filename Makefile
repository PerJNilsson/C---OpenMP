all: 	read_file

read_file:	read_file.c
	gcc -o read_file read_file.c -march=native -O2
