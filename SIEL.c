#include "Matrix/matrix.h"
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

	Matrix *mAB, *mA, *mB;
	mAB = fileToMatrix(argv[1]);
	getABfromMatrix(mAB, &mA, &mB);

	puts("Matrix AB:");
	printMatrix(mAB);

	puts("\nMatrix A:");
	printMatrix(mA);
	printDom(mA);

	puts("\nMatrix B:");
	printMatrix(mB);

	freeMatrix(mAB);
	freeMatrix(mA);
	freeMatrix(mB);

	return 0;
}
