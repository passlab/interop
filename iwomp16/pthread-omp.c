#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

void *omp_parallel_foo(void *ptr);

int main(int argc, char * argv[])
{
     pthread_t thread1, thread2;
     const char *message1 = "pthread 1";
     const char *message2 = "pthread 2";

    /* Create independent threads each of which will execute function */
     pthread_create(&thread1, NULL, omp_parallel_foo, (void*) message1);
     pthread_create(&thread2, NULL, omp_parallel_foo, (void*) message2);

     omp_parallel_foo("master thread");

     /* Wait till threads are complete before main continues */
     pthread_join(thread1, NULL);
     pthread_join(thread2, NULL);
     return 0;
}

void *omp_parallel_foo(void *ptr )
{
    char *message = (char *) ptr;
#pragma omp parallel shared(message)
    {
	int id = omp_get_thread_num();
	printf("%s: OMP thread: %d,  \n", message, id);
    }
    return NULL;
}
