#include "Matrix/matrix.h"

int main(int argc, char * argv[])
{
	if( argc != 2 )
	{
		puts(	"Modo de uso:\n"
			"\t./SIEL <matrizAB.txt>\n");

		return -1;
	}

	Matrix * mAB;
	mAB = fileToMatrixAB(argv[1]);
	printMatrix(mAB);
	freeMatrix(mAB);

	return 0;
}
