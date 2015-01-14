#include <stdio.h>
#include <stdlib.h>
#include "omp.h"

void omp_msg_icc(char *ptr) {
	char *message = (char *) ptr;
#pragma omp parallel shared(message)
	{
		int id = omp_get_thread_num();
		printf("OMP thread %d of func %s: %s\n", id, __func__, message);
	}
}
