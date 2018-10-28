#include "../matrix.h"
#include <stdio.h>

Matrix * jacobi_X(Matrix * mXprev, Matrix * mT, Matrix * mC)
{
	Matrix * Xk, * mTXprev;
	mTXprev = matrix_mult( mT, mXprev );
	Xk = matrix_sum( mTXprev, mC);
	
	freeMatrix(mTXprev);
	return Xk;
}

void jacobi_getTCfromAB( Matrix * mA, Matrix * mB, Matrix ** mT, Matrix ** mC)
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

void jacobi_solve(Matrix * mA, Matrix * mB, Matrix * mX0)
{
	Matrix *mT, *mC, *mX = mX0, *mXprev;

	jacobi_getTCfromAB(mA,mB,&mT,&mC);


	puts("MATRIZ T:");
	printMatrix(mT);

	puts("\nMATRIZ C");
	printMatrix(mC);

	puts("\nVECTOR INICIAL");
	printMatrix(mX0);

	puts("\nRESOLUCION EN 10 ITERACIONES");

	int i;
	for( i = 0; i < 10; i++ )
	{
		puts("\n======================");
		printf("\nVECTOR X%d:\n", i);
		printMatrix(mX);
		puts("\n======================");
		mXprev = mX;
		mX = jacobi_X(mXprev, mT, mC);
		freeMatrix(mXprev);
	}
	puts("\n===RESULTADO FINAL===");
	printMatrix(mX);
	freeMatrix(mX);
}
