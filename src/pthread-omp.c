#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

void *omp_parallel_foo(void *ptr);

/**Important: make sure you use num_threads clause in parallel direction and set it to the 
 * number of hardware cores, not the number of cores Linux gives or the default from OpenMP
 */
int main(int argc, char * argv[])
{
     pthread_t thread1, thread2;
     const char *message1 = "pthread 1";
     const char *message2 = "pthread 2";

    /* Create independent threads each of which will execute function */
     //pthread_create(&thread1, NULL, omp_parallel_foo, (void*) 1);
     pthread_create(&thread2, NULL, omp_parallel_foo, (void*) 2);

     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */

     omp_parallel_foo((void*)1);

     pthread_join(thread1, NULL);
     pthread_join(thread2, NULL);

     exit(0);
}

void busy_waiting(int second) {
	sleep(second); /* temporary implem */
}

void omp_quiesce() {
	__kmpc_end();
}

#define NUM_ITERATIONS 100000000

void *omp_parallel_foo(void *ptr )
{
    int user_thread_id = (int) ptr;
    int i;
    for (i=0; i<NUM_ITERATIONS; i++) {
    	/* busy wait for one second */
    	busy_waiting(user_thred_id);
#pragma omp parallel shared(message)
	{
		int id = omp_get_thread_num();
		printf("%d OMP thread for user thread %d,  \n", message, id, user_thread_id);
		/* busy waiting, the whole parallel region should takes 1s */
		busy_waiting(1);
	}
	omp_set_wait_policy(PASSIVE); /* or active */
	/* or omp_quiesce() */

    }
	return NULL;
}

void *omp_quiesce_overhead(void *ptr )
{
    int user_thread_id = (int) ptr;
    int i;
    double total = read_timer();
    for (i=0; i<NUM_ITERATIONS; i++) {
#pragma omp parallel shared(message) firstprivate(omp_start_cost)
	{
		
	}
	omp_quiesce();
     }
     total = read_timer - total;
     
     double parallel_overhead = read_timer();
     for (i=0; i<NUM_ITERATIONS; i++) {
#pragma omp parallel shared(message)
	{
	}
     }
     parallel_overhead = read_timer() - parallel_overhead;
     
     double quiesce_startup = total - parallel_overhead; /* this includes both the startup and quiesce */
     
     return NULL;
}
