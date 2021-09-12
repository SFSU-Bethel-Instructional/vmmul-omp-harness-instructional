#include <cblas.h>

const char* dgemv_desc = "Reference dgemv.";

/*
 * This routine performs a dgemv operation
 * Y :=  A * X + Y
 * where A is lda-by-lda matrix stored in row-major format, and X and Y are lda by 1 vectors.
 * On exit, A and X maintain their input values.
 * This function wraps a call to the BLAS-2 routine DGEMV
 */
void my_dgemv(int n, double* A, double* x, double* y) {
   double alpha=1.0, beta=1.0;
   int lda=n, incx=1, incy=1;
   cblas_dgemv(CblasRowMajor, CblasNoTrans, n, n, alpha, A, lda, x, incx, beta, y, incy);
}
