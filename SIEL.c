#include "Matrix/matrix.h"
#include "Matrix/Metodos/metodo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * getUserInput(void)
{
	char c, * userInput = NULL;
	int i = 0;

	while( ( c = getchar() ) != '\n' )
	{
		userInput = realloc( userInput, i + 1 );
		userInput[i++] = c;
	}

	userInput = realloc( userInput, i + 1 );
	userInput[i] = '\0';

	return userInput;
}

Matrix * obtenerMatrizDeUsuario()
{
	printf(
	"===== SIEL =====\n"
	"\nINGRESE EL PATH DE SU ARCHIVO CON LA MATRIZ AB PARA INICIAR\n"
	"\nINGRESELO AQUI --->"
	);

	char * path;
	path = getUserInput();

	Matrix * mAB;

	do
	{
		mAB = fileToMatrix(path);
		if(!mAB)
		{
			puts("No se pudo leer el archivo");
			puts(
			"Ingrese otro path o "
			"presione enter para salir."
			);

			path = getUserInput();

			if( !strcmp(path,"") )
			{
				puts("Saliendo de SIEL");
				exit(-1);
			}
		}
		else
		{	
			char * rta;
			puts("Ud. desea ingresar esta matriz:");
			printMatrix(mAB);
			puts("[y/n]");
			do
			{
				rta = getUserInput();
				if( !strcmp(rta, "n") )
				{
					freeMatrix(mAB);
					mAB = NULL;
					puts(
					"Ingrese otro path o presione "
					"enter para salir."
					);

					path = getUserInput();

					if( !strcmp(path,"") )
					{
						puts("Saliendo de SIEL");
						exit(-1);
					}
				}
			}
			while( strcmp(rta, "y") && strcmp(rta, "n"));
		}
	}	
	while(!mAB );

	puts("Genial.");

	return mAB;
}



int main(int argc, char * argv[])
{	
	Matrix * mAB;
	mAB = obtenerMatrizDeUsuario();

	freeMatrix(mAB);


	/*
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
	
	Matrix * vIni =  matrix_new(mA->rows, 1);
	vIni->data[0][0] = 1;
	vIni->data[1][0] = 1;
	vIni->data[2][0] = 1;

	metodo_resolver(mA, mB, vIni, jacobi_X);

	freeMatrix(mAB);
	freeMatrix(mA);
	freeMatrix(mB);
	//freeMatrix(mT);
	//freeMatrix(mC);
	*/
	return 0;
}
