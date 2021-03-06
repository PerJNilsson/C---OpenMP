.PHONY : all

all : clean cell_distance

submit : clean test

cell_distance : cell_distance
	gcc -g -o cell_distance cell_distance.c -lm -fopenmp -O3

clean : clean
	@clear; rm -rf cell_distance; rm -rf cell_distance; rm -rf cell_distance.tar.gz; rm -rf distances; rm -rf extracted; rm -rf reports;
test :
	@mkdir cell_distance && cp cell_distance.c cell_distance/cell_distance.c && cp makefile \
	cell_distance/makefile && cp report.md cell_distance/report.md
	cd cell_distance && tar czvf ../cell_distance.tar.gz . && cd -
	/home/hpc2018/a3_grading/check_submission.py ~/assignment3/cell_distance.tar.gz
