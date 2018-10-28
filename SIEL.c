#include "Matrix/matrix.h"
#include "Matrix/Metodos/jacobi.h"
#include <stdio.h>

void printDom(Matrix * m)
{
	int nivelDom;
	if( nivelDom = diagonalmenteDominante(m) )
	{
		switch( nivelDom )
		{
			case SIMPLE:
				puts("Matriz diagonalmente dominante");
				break;
			case ESTRICTA:
				puts("Matrix diag. dom. estricta");
				break;
			default:
				puts("Error: nivelDom no reconocido");
		}
	}
	else
		puts("Matriz no regular");
}


int main(int argc, char * argv[])
{
	if( argc != 2 )
	{
		puts(	"Modo de uso:\n"
			"\t./SIEL <matrizAB.txt>\n");

		return -1;
	}

	Matrix *mAB, *mA, *mB, *mT, *mC, *rtdo;
	mAB = fileToMatrix(argv[1]);
	getABfromMatrix(mAB, &mA, &mB);

	puts("\nMatrix A:");
	printMatrix(mA);
	printDom(mA);

	puts("\nMatrix B:");
	printMatrix(mB);

/*	jacobi_getTCfromAB(mA,mB,&mT,&mC);

	puts("\nMatrix T:");
	printMatrix(mT);

	puts("\nMatrix C:");
	printMatrix(mC);
*/

	jacobi_solve(mA, mB, matrix_new(mA->rows, 1) );

	freeMatrix(mAB);
	freeMatrix(mA);
	freeMatrix(mB);
	//freeMatrix(mT);
	//freeMatrix(mC);

	return 0;
}
