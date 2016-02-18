/*
 * AXPY  Y[N] = Y[N] + a*X[N]
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/timeb.h>
#include <cilk/cilk.h> 

/* read timer in second */
double read_timer() {
    struct timeb tm;
    ftime(&tm);
    return (double) tm.time + (double) tm.millitm / 1000.0;
}

/* read timer in ms */
double read_timer_ms() {
    struct timeb tm;
    ftime(&tm);
    return (double) tm.time * 1000.0 + (double) tm.millitm;
}

#define REAL float
#define VECTOR_LENGTH 102400

/* initialize a vector with random floating point numbers */
void init(REAL *A, int N) {
    int i;
    for (i = 0; i < N; i++) {
        A[i] = (double) drand48();
    }
}

double check(REAL *A, REAL B[], int N) {
    int i;
    double sum = 0.0;
    for (i = 0; i < N; i++) {
        sum += A[i] - B[i];
    }
    return sum;
}

void axpy_base(int N, REAL *Y, REAL *X, REAL a);
void axpy_cilkplus(int N, REAL *Y, REAL *X, REAL a);
void axpy_cilkplus_for(int N, REAL *Y, REAL *X, REAL a);
void axpy_cilk_for(int N, REAL *Y, REAL *X, REAL a);

int main(int argc, char *argv[]) {
    int N = VECTOR_LENGTH;
    int num_threads = 4; /* 4 is default number of threads */
    if (argc < 2) {
        fprintf(stderr, "Usage: axpy <n> (default %d) [<num_threads>] (default %d)\n", N, num_threads);
        exit(1);
    }   
    N = atoi(argv[1]);
    if (argc >=3) num_threads = atoi(argv[2]);
    char cilk_nworkers[3];
    sprintf(cilk_nworkers, "%d", num_threads);
    __cilkrts_set_param("nworkers", cilk_nworkers);

    REAL a = 123.456;
    REAL *Y_base = malloc(sizeof(REAL)*N);
    REAL *Y_parallel = malloc(sizeof(REAL)*N);
    REAL *X = malloc(sizeof(REAL)* N);

    srand48((1 << 12));
    init(X, N);
    init(Y_base, N);
    memcpy(Y_parallel, Y_base, N * sizeof(REAL));

    /* example run */
    double elapsed = read_timer();
    axpy_base(N, Y_base, X, a);
    elapsed = (read_timer() - elapsed);

    int i;
    int num_runs = 10;
    double elapsed_cilkplus = read_timer();
    for (i=0; i<num_runs; i++) axpy_cilkplus(N, Y_parallel, X, a);
    elapsed_cilkplus = (read_timer() - elapsed_cilkplus)/num_runs;
    
    double elapsed_cilkplus_for = read_timer();
    for (i=0; i<num_runs; i++) axpy_cilkplus_for(N, Y_parallel, X, a);
    elapsed_cilkplus_for = (read_timer() - elapsed_cilkplus_for)/num_runs;
    
    double elapsed_cilk_for = read_timer();
    for (i=0; i<num_runs; i++) axpy_cilk_for(N, Y_parallel, X, a);
    elapsed_cilk_for = (read_timer() - elapsed_cilk_for)/num_runs;

    /* you should add the call to each function and time the execution */
    printf("======================================================================================================\n");
    printf("\tAXPY: Y[N] = Y[N] + a*X[N], N=%d, %d threads for dist\n", N, num_threads);
    printf("------------------------------------------------------------------------------------------------------\n");
    printf("Performance:\t\t\tRuntime (ms)\t MFLOPS \t\tError (compared to base)\n");
    printf("------------------------------------------------------------------------------------------------------\n");
    printf("axpy_base:\t\t\t%4f\t%4f \t\t%g\n", elapsed * 1.0e3, (2.0 * N) / (1.0e6 * elapsed), check(Y_base, Y_base, N));
    printf("axpy_cilkplus:\t\t\t%4f\t%4f \t\t%g\n", elapsed_cilkplus * 1.0e3, (2.0 * N) / (1.0e6 * elapsed_cilkplus), check(Y_base,
                                                                                                              Y_parallel, N));
    printf("axpy_cilkplus_for:\t\t%4f\t%4f \t\t%g\n", elapsed_cilkplus_for * 1.0e3, (2.0 * N) / (1.0e6 * elapsed_cilkplus_for), check(Y_base,
                                                                                                              Y_parallel, N));
    printf("axpy_cilk_for:\t\t\t%4f\t%4f \t\t%g\n", elapsed_cilk_for * 1.0e3, (2.0 * N) / (1.0e6 * elapsed_cilk_for), check(Y_base,
                                                                                                              Y_parallel, N));

    free(Y_base);
    free(Y_parallel);
    free(X);

    return 0;
}

void axpy_base(int N, REAL *Y, REAL *X, REAL a) {
    int i;
    for (i = 0; i < N; ++i)
        Y[i] += a * X[i];
}

void axpy_sub(int start, int end, REAL *Y, REAL *X, REAL a) {
    int i;
    for (i = start; i < end; ++i)
        Y[i] += a * X[i];
}

#define BASE 32
void axpy_cilkplus(int N, REAL *Y, REAL *X, REAL a) {
    if (N<=BASE) {
       int i;
       for (i = 0; i < N; ++i)
          Y[i] += a * X[i];
    } else {
       cilk_spawn axpy_cilkplus(N/2, Y, X, a);
       axpy_cilkplus(N-N/2, Y+N/2, X+N/2, a);
       cilk_sync;
    }
}

void axpy_cilkplus_for(int N, REAL *Y, REAL *X, REAL a) {
    int i;
    int nworkers = __cilkrts_get_nworkers();
    int Nt = N/nworkers;
    for (i=0; i<nworkers; i++) {
	int start, end;
	start = i*Nt;
	end = (i+1)*Nt;
        cilk_spawn axpy_sub(start, end, Y, X, a);
    }
    cilk_sync;
}

void axpy_cilk_for(int N, REAL *Y, REAL *X, REAL a) {
    int i;
    cilk_for (i = 0; i < N; ++i)
        Y[i] += a * X[i];
}
/*
 * ssh -l<netid> lennon.secs.oakland.edu
 * . /opt/intel/bin/iccvars.sh -arch intel64 -platform linux
 *
 * icc axpy-omp-cilkplus.c -openmp -o axpy
 *
 */
