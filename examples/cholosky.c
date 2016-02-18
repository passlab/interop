void cholesky_factorization(const int ts, const int nt, double* A[nt][nt]) {
#pragma omp parallel
#pragma omp single
for (int k = 0; k < nt; k++) {
 Diagonal Block factorization
#pragma omp task depend(inout:A[k][k])
 dpotrf(&L, &ts, A[k][k], &ts, &INFO);
 // Triangular systems
 for (int i = k + 1; i < nt; i++) {
 #pragma omp task depend(in:A[k][k]) depend(inout: A[k][i])
 dtrsm(&RI, &LO, &TR, &NU, &ts, &ts, &DONE, A[k][k], &ts, A[k][i], &ts);
 }
 // Update trailing matrix
 for (int i = k + 1; i < nt; i++) { for (int j = k + 1; j < i; j++) {
 #pragma omp task depend(in:A[k][i],A[k][j]) depend(inout:A[j][i]))
 dgemm(&NT, &TR, &ts, &ts, &ts, &DMONE, A[k][i], &ts, A[k][j], &ts, &DONE, A[j][i], &ts);
 }
 #pragma omp task depend(in:A[k][i]) depend(inout:A[i][i]))
 dsyrk(&LO, &NT, &ts, &ts, &DMONE, A[k][i], &ts, &DONE, A[i][i], &ts); }
 } }
