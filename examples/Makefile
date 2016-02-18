
default: fib_cilkplus fib_omp axpy_omp axpy_cilkplus

fib_cilkplus: fib_cilkplus.c
	icc -O0 fib_cilkplus.c -o fib_cilkplus-icc

fib_omp: fib_omp.c
	icc -O0 -openmp fib_omp.c -o fib_omp-icc

axpy_omp: axpy_omp.c
	icc -O0 -openmp axpy_omp.c -o axpy_omp-icc

axpy_cilkplus: axpy_cilkplus.c
	icc -O0 axpy_cilkplus.c -o axpy_cilkplus-icc

clean:
	rm -rf ${OBJS} fib *.log
