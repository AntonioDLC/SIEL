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

void metodo_resolver(Matrix * mA, Matrix * mB, Matrix * mX0, Metodo_X metodo_X, float cota_error)
{
	Matrix *mT, *mC, *mX = mX0, *mXprev;
	float ult_error, norma_anterior;

	metodo_getTCfromAB(mA,mB,&mT,&mC);


	puts("MATRIZ T:");
	printMatrix(mT);

	puts("\nMATRIZ C");
	printMatrix(mC);

	puts("\nVECTOR INICIAL");
	printMatrix(mX0);

	puts("\nRESOLUCION");

	int i;
	for( i = 0; ult_error < cota_error; i++ )
	{
		puts("\n======================");
		printf("\nVECTOR X%d:\n", i);
		printMatrix(mX);
		puts("\n======================");
		mXprev = mX;
		mX = metodo_X(mXprev, mT, mC);
		norma_anterior = calcularNormaInf(mXprev);
		freeMatrix(mXprev);
		ult_error = fabs(  calcularNormaInf(mX) - norma_anterior );
	}
	puts("\n===RESULTADO FINAL===");
	printMatrix(mX);
	freeMatrix(mX);
}
