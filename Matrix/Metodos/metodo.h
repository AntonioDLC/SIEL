#ifndef _METODO_H_
#define _METODO_H_

typedef Matrix * Metodo_X (Matrix *Xp, Matrix *T, Matrix *C);

Metodo_X jacobi_X;
Metodo_X GS_X;

void metodo_getTCfromAB(Matrix*A,Matrix*B,Matrix**T,Matrix**C);
void metodo_resolver(Matrix *A, Matrix *B, Matrix *X0, Metodo_X met_X, float cerror);
#endif
