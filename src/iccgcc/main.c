#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

void omp_msg_icc(char *ptr);
void omp_msg_gcc(char *ptr);

int main(int argc, char * argv[])
{
     char *msg = "Hello World!";

     omp_msg_icc(msg);
     printf("from master: %d\n", omp_get_thread_num());
     omp_msg_gcc(msg);

     return 0;
}

void omp_msg_gcc(char *ptr) {
	char *message = (char *) ptr;
#pragma omp parallel shared(message) num_threads(4)
	{
		int id = omp_get_thread_num();
		printf("OMP thread %d of func %s: %s\n", id, __func__, message);
	}
}
