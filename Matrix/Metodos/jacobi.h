#ifndef _JACOBI_H_
#define _JACOBI_H_

void jacobi_getTCfromAB(Matrix*A,Matrix*B,Matrix**T,Matrix**C);
void jacobi_solve(Matrix *A, Matrix *B, Matrix *X0);
#endif
