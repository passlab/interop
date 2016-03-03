// example for thread::join
#include <iostream>       // std::cout
#include <chrono>         // std::chrono::seconds
#include <future>
#include <pthread.h>
#include <thread>  
#include <future>
#include <sys/timeb.h>

const int NUM_THREADS=5;
//#define NUM_THREADS 5
const int vector_length=102400;
#define BASE 4
#define REAL float

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

void init(REAL *A, int N){
int i;
for (i=0; i<N; i++) {
A[i]= (double) drand48();
}
}


double check(REAL A[], REAL B[], int N) {
    int i;
    double sum = 0.0;
    for (i = 0; i < N; i++) {
        sum += A[i] - B[i];
    }
    return sum;
}

//===========================async-future

/*void axpy_c_asy_task(int N, REAL *Y, REAL *X, REAL a) {

 if (N<=BASE) {
       int i;
       for (i = 0; i < N; ++i)
          Y[i] += a * X[i];
            } else {

   std::future<float> f1  = async (launch::async,axpy_omp_task,N/2, Y, X, a);
         axpy_omp_task(N-N/2, Y+N/2, X+N/2, a);
     
                   }
    }*/   
//===========================
void axpy_cmth1(int N, REAL *Y, REAL *X, REAL a, int NUM_THREADS) {
    int i;
   // int r[num_threads];
   // pthread_t threads[num_threads];
    
    for (i=0; i<NUM_THREADS; i++) 
    {   
        int tid = i;
        int end, start, Nt;
        Nt = N/NUM_THREADS;
        start = tid*Nt;
        end = (tid+1)*Nt;
        int j;
      for (j=start; j<end; j++)
                Y[i] += a * X[i];
                  }
    }
//===========================
void axpy_cmth(int N, REAL *Y, REAL *X, REAL a, int Num_THREADS) {
    int i;
    for (i = 0; i < N; ++i)
        Y[i] += a * X[i];     
     }

void axpy_cmth1(int N, REAL *Y, REAL *X, REAL a, int NUM_THREADS);
 void axpy_c_asy_task(int N, REAL *Y, REAL *X, REAL a);

//===========================          
int main() 
{
 int N=vector_length;
REAL X[N];
REAL Y[N];
REAL a=0.1234;
double elapsed;
double elapsed_dist;
std::thread thread[NUM_THREADS];
elapsed=read_timer();
 
//void axpy_cmth1(int N, REAL *Y, REAL *X, REAL a, int NUM_THREADS);
// printf("Main: creating thread %ld\n", NUM_THREADS);
 int i=0,j=0;
 for (i=0; i<NUM_THREADS;++i)
 thread[i]=std::thread(axpy_cmth1, N,(REAL*)Y, (REAL *)X, a,NUM_THREADS);     // spawn new thread 
 // std::thread second (axpy_cmth1, N,Y, X, a,num_threads);     // spawn new thread 
for(j=0;j<NUM_THREADS;++j)
  thread[j].join();// join the first thread 
elapsed=(read_timer() - elapsed); 
// second.join();
 //std::cout << "Calculating. Please, wait...\n";
// std::future<bool> fut = std::async (is_prime,313222313);
 /* you should add the call to each function and time the execution */
    printf("======================================================================================================\n");
    printf("\tAXPY: Y[N] = Y[N] + a*X[N], N=%d, %d tasks for dist\n", N, NUM_THREADS);
    printf("------------------------------------------------------------------------------------------------------\n");
    printf("Performance:\t\tRuntime (ms)\t MFLOPS \t\tError (compared to base)\n");
    printf("------------------------------------------------------------------------------------------------------\n");
    printf("axpy_base:\t\t%4f\t%4f \t\t%g\n", elapsed * 1.0e3, (2.0 * N) / (1.0e6 * elapsed), check(Y, Y, N));
  


return 0;
}
