#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

double read_timer_ms() {
    struct timeval t;
    gettimeofday(&t, 0);
    return t.tv_sec * 1000ULL + t.tv_usec / 1000ULL;
}

double read_timer() {
    return read_timer_ms() * 1.0e-3;
}

int fib(int n)
{
  int x, y;
  if (n<2)
    return n;
  else
    {
       #pragma omp task shared(x) firstprivate(n)
       x=fib(n-1);

       y=fib(n-2);

       #pragma omp taskwait
       return x+y;
    }
}

int main(int argc, char * argv[])
{
  int n, result;

  if (argc < 2) {
        fprintf(stderr, "Usage: fib <n> [<#threads>]\n");
        exit(1);
  }
  n = atoi(argv[1]);
  int num_threads;
  if (argc > 2) {
	num_threads  = atoi(argv[2]);
	omp_set_num_threads(num_threads);
  }

  double tm_elaps; 

  n = atoi(argv[1]);
  #pragma omp parallel shared(n, num_threads, result, tm_elaps)
  {
//    int id = omp_get_thread_num();
    #pragma omp single
    {
    	num_threads = omp_get_num_threads();
	int i;
	int totalrun = 10;
	double tm_begin = read_timer();
	for (i=0; i<totalrun; i++)
	        result = fib(n);
	tm_elaps = read_timer() - tm_begin;
	tm_elaps = tm_elaps/totalrun;
    }
  }
  printf ("fib(%d) = %d\n", n, fib(n));
  printf("Time (fib(%d) with %d threads): %f seconds\n", n, num_threads, (double)(tm_elaps));
  return 0;
}
