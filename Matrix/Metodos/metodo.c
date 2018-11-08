#include "../matrix.h"
#include "metodo.h"
#include <stdio.h>
#include <math.h>

Matrix * GS_X(Matrix * mXprev, Matrix * mT, Matrix * mC)
{
	Matrix * Xk = matrix_copy(mXprev);

	int i, k;
	for( i = 0; i < mT->rows; i++)
	{
		float sum = 0;
		for( k = 0; k < mT->columns; k++)
			sum += mT->data[i][k] * Xk->data[k][0];

		sum += mC->data[i][0];

		Xk->data[i][0] = sum;
	}

	return Xk;
}

Matrix * jacobi_X(Matrix * mXprev, Matrix * mT, Matrix * mC)
{
	Matrix * Xk, * mTXprev;
	mTXprev = matrix_mult( mT, mXprev );
	Xk = matrix_sum( mTXprev, mC);
	
	freeMatrix(mTXprev);
	return Xk;
}

void metodo_getTCfromAB( Matrix * mA, Matrix * mB, Matrix ** mT, Matrix ** mC)
{
	*mT = matrix_new( mA->rows, mA->columns);
	*mC = matrix_new( mB->rows, mB->columns);

	int i,j;

	for( i = 0; i < (*mT)->rows; i++)
	{
		float elem = mA->data[i][i];
		for( j = 0; j < (*mT)->columns; j++)
		{
			if( j == i )
				(*mT)->data[i][j] = 0;
			else
				(*mT)->data[i][j] = -mA->data[i][j] / elem;
		}

		(*mC)->data[i][0] = mB->data[i][0] / elem;
	}
}

void metodo_resolver(Matrix * mA, Matrix * mB, Matrix * mX0, Metodo_X metodo_X, float cota_error, int cant_decs)
{
	Matrix *mT, *mC, *mX = matrix_copy(mX0), *mXprev;
	float ult_error = 100, norma, norma_anterior;

	metodo_getTCfromAB(mA,mB,&mT,&mC);


	puts("MATRIZ T:");
	printMatrix(mT);

	puts("\nMATRIZ C");
	printMatrix(mC);

	puts("\nVECTOR INICIAL");
	printVector(mX0, cant_decs);

	puts("\nRESOLUCION");

	int i;
	norma_anterior = calcularNormaInf(mX);
	for( i = 0; ult_error > cota_error; i++ )
	{
		puts("======================");
		printf("VECTOR X%d: ", i);
		printVector(mX, cant_decs);
		mXprev = mX;
		mX = metodo_X(mXprev, mT, mC);
		norma = calcularNormaInf(mX);
		freeMatrix(mXprev);
		ult_error = fabs(  norma - norma_anterior );
		printf(	"\nCRITERIO DE PARO: "
			"Diferencia entre norma infinita actual: %f\n"
			"...y la anterior: %f\n"
			"Da: %f, y la cota de error es %f",
			norma, norma_anterior, ult_error, cota_error);

		norma_anterior = norma;
		getchar();
	}
	puts("\n===RESULTADO FINAL===");
	printVector(mX, cant_decs);
	putchar('\n');
	freeMatrix(mX);
}
