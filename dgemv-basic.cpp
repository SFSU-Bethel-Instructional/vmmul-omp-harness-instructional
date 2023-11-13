const char *dgemv_desc = "Basic implementation of matrix-vector multiply.";

/*
 * This routine performs a dgemv operation
 * Y :=  A * X + Y
 * where A is n-by-n matrix stored in row-major format, and X and Y are n by 1 vectors.
 * On exit, A and X maintain their input values.
 */
void my_dgemv(int n, double *A, double *x, double *y)
{
   // insert your code here: implementation of basic matrix multiply
   for (int row = 0; row < n; row++)
   {
      int offset = row * n;
      for (int column = 0; column < n; column++)
      {
         y[offset + column] += A[row * n + column] * x[column];
      }
   }
}