default:
	gcc -fopenmp -c main.c
	icc -openmp -c omp_msg_icc.c
	gcc main.o omp_msg_icc.o -L/opt/intel/composer_xe_2013_sp1.1.106/compiler/lib/intel64 -liomp5 -o main
	#gcc main.o omp_msg_icc.o -L/opt/intel/composer_xe_2013_sp1.1.106/compiler/lib/intel64 -liomp5 -lgomp -Bsymbolic -o main

