#ifndef _MATRIX_H_
#define _MATRIX_H_

typedef float ** MatrixData;

typedef struct
	{
		MatrixData data;
		int rows;
		int columns;
	} Matrix;

typedef enum{ NO_DOMINANTE, SIMPLE, ESTRICTA} Dominancia;

Matrix * fileToMatrix(char * path);
Matrix * matrix_mult(Matrix *, Matrix *);
void getABfromMatrix(Matrix * m, Matrix ** a, Matrix ** b);
void printMatrix(Matrix * m);
void freeMatrix(Matrix * m);
int diagonalmenteDominante(Matrix * m);

#endif
