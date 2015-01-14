#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "omp.h"

void omp_msg_icc(char *ptr);
void omp_msg_gcc(char *ptr);

void omp_msg_gcc(char *ptr) {
	char *message = (char *) ptr;
#pragma omp parallel shared(message)
	{
		int id = omp_get_thread_num();
		printf("OMP thread %d of func %s: %s\n", id, __func__, message);
	}
}

int main(int argc, char * argv[])
{
     pthread_t thread1, thread2;

    /* Create independent threads each of which will execute function */
     pthread_create(&thread1, NULL, omp_msg_icc, (void*)"pthread icc");
     pthread_create(&thread2, NULL, omp_msg_gcc, (void*)"pthread gcc");

     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */

     pthread_join(thread1, NULL);
     pthread_join(thread2, NULL);

     return 0;
}
