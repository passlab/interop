/***************************************************************************

                  COPYRIGHT

The following is a notice of limited availability of the code, and disclaimer
which must be included in the prologue of the code and in all source listings
of the code.

Copyright Notice
 + 2014 Argonne National Laboratory

Permission is hereby granted to use, reproduce, prepare derivative works, and
to redistribute to others.  This software was authored by:

Jeff R. Hammond
Leadership Computing Facility
Argonne National Laboratory
Argonne IL 60439 USA
e-mail: jhammond@anl.gov

                  GOVERNMENT LICENSE

Portions of this material resulted from work developed under a U.S.
Government Contract and are subject to the following license: the Government
is granted for itself and others acting on its behalf a paid-up, nonexclusive,
irrevocable worldwide license in this computer software to reproduce, prepare
derivative works, and perform publicly and display publicly.

                  DISCLAIMER

This computer code material was prepared, in part, as an account of work
sponsored by an agency of the United States Government.  Neither the United
States, nor the University of Chicago, nor any of their employees, makes any
warranty express or implied, or assumes any legal liability or responsibility
for the accuracy, completeness, or usefulness of any information, apparatus,
product, or process disclosed, or represents that its use would not infringe
privately owned rights.

 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <omp.h>

int num_posix_threads;
pthread_t * thread_pool;

#ifdef __APPLE__
typedef struct {
    volatile int c;
    int n;
} stupid_barrier_t;

stupid_barrier_t barrier;

int stupid_barrier_init(stupid_barrier_t * barrier, void * junk, int num_threads)
{
    assert(num_threads>0);
    barrier->c = num_threads;
    barrier->n = num_threads;
    return 0;
}

int stupid_barrier_destroy(stupid_barrier_t * barrier)
{
    barrier->c = 0;
    barrier->n = 0;
    return 0;
}

int stupid_barrier_wait(stupid_barrier_t * barrier)
{
    size_t id = (size_t)pthread_self();

    printf("%zu: before barrier on %d threads \n", id, barrier->n);

    int ticket = __sync_fetch_and_sub(&(barrier->c),1);

    if (ticket==1) {
        barrier->c = barrier->n;
    } else {
        while(barrier->c != 0);
    }

    printf("%zu: after barrier on %d threads \n", id, barrier->n);

    return 0;
}
#else
pthread_barrier_t barrier;
pthread_barrierattr_t barrier_attr;
#endif

void* foo(void* dummy)
{
    int my_pth;
    pthread_t my_pthread = pthread_self();

    for (int i=0 ; i<num_posix_threads ; i++)
        if (my_pthread==thread_pool[i])
            my_pth = i;
    fprintf(stdout, "hello from pthread %d!\n" , my_pth );
    fflush(stdout);

    int rc;
#ifdef __APPLE__
    rc = stupid_barrier_wait(&barrier);
#else
    rc = pthread_barrier_wait(&barrier);
#endif
    assert(rc==0);

    int my_omp, num_omp, max_omp;
    #pragma omp parallel private(my_omp,num_omp,max_omp) shared(my_pth)
    {
        my_omp  = omp_get_thread_num();
        num_omp = omp_get_num_threads();
        max_omp = omp_get_max_threads();
        fprintf(stdout,"hello from OpenMP thread %d of %d (max=%d) on pthread %d\n",my_omp,num_omp,max_omp,my_pth);
        fflush(stdout);
    }

    pthread_exit(0);
}

int main(int argc, char** argv)
{
    char * char_posix_threads = getenv("POSIX_NUM_THREADS");
    num_posix_threads = (char_posix_threads!=NULL) ? atoi(char_posix_threads) : 4;

    fprintf(stdout,"num_posix_threads = %d\n",num_posix_threads);
    fprintf(stdout,"omp_get_max_threads() = %d\n",omp_get_max_threads());
    fprintf(stdout,"creating %d threads\n",num_posix_threads);
    fflush(stdout);

    thread_pool = (pthread_t*) malloc(num_posix_threads*sizeof(pthread_t));
    assert(thread_pool!=NULL);

    int rc;
#ifdef __APPLE__
    rc = stupid_barrier_init(&barrier, NULL, num_posix_threads);
#else
    rc = pthread_barrier_init(&barrier, &barrier_attr, num_posix_threads);
#endif
    assert(rc==0);

    for (int i=0 ; i<num_posix_threads ; i++){
        rc = pthread_create(&thread_pool[i], NULL, foo, NULL);
        assert(rc==0);
    }

    for (int i=0 ; i<num_posix_threads ; i++){
        rc = pthread_join(thread_pool[i],NULL);
        assert(rc==0);
    }
    fprintf(stdout,"joined %d threads\n", num_posix_threads);
    fflush(stdout);

#ifdef __APPLE__
    rc = stupid_barrier_destroy(&barrier);
    assert(rc==0);
#endif

    free(thread_pool);

    return(0);
}

