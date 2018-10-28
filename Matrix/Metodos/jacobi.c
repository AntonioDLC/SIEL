#include "../matrix.h"

Matrix * jacobi_Xk( int k, Matrix * mX0, Matrix * mT, Matrix * mC)
{
	return (void*)0;
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
				(*mT)->data[i][j] = mA->data[i][j] / elem;
		}

		(*mC)->data[i][0] = mB->data[i][0] / elem;
	}
}
