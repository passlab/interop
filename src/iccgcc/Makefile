default:
	gcc -fopenmp -save-temps -c main.c
	icc -openmp -save-temps -c omp_msg_icc.c
	gcc main.o omp_msg_icc.o -L/opt/intel/composer_xe_2013_sp1.1.106/compiler/lib/intel64 -lgomp -liomp5 -Bsymbolic -o main
#	gcc main.o omp_msg_icc.o -L/opt/intel/composer_xe_2013_sp1.1.106/compiler/lib/intel64 -L/opt/local/gcc/4.7.2/lib64 -liomp5 -lgomp -t -o main
	#gcc main.o omp_msg_icc.o -L/opt/intel/composer_xe_2013_sp1.1.106/compiler/lib/intel64 -L/opt/local/gcc/4.7.2/lib64 -liomp5 -lgomp -Bsymbolic -o main
	#gcc main.o omp_msg_icc.o -L/opt/intel/composer_xe_2013_sp1.1.106/compiler/lib/intel64 -liomp5 -Bsymbolic -o main

