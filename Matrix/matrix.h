#ifndef _MATRIX_H_
#define _MATRIX_H_

typedef float ** MatrixData;

typedef struct
	{
		MatrixData data;
		int rows;
		int columns;
	} Matrix;

char * fileToMatrix(char * path);
void printMatrix(Matrix * m);
void freeMatrix(Matrix * m);

#endif
