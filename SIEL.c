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
	"\nINGRESE EL PATH A SU MATRIZ AQUI --->"
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

Metodo_X * obtenerMetodoDeUsuario(void)
{
	puts("INGRESE EL NRO DEL METODO A UTILIZAR");
	puts("1. Jacobi");
	puts("2. Gauss-Seidel");

	char * rta;
	Metodo_X * met;

	do
	{
		rta = getUserInput();
	
		if( !strcmp("1", rta) )
		{
			puts("Ud. selecciono Jacobi");
			met = jacobi_X;
		}
		else
		if( !strcmp("2", rta) )
		{
			puts("Ud. selecciono Gauss-Seidel");
			met = GS_X;
		}
		else
		{
			puts("Opcion incorrecta");
			met = NULL;
		}
	}
	while(!met);

	return met;
}

Matrix * obtenerVectorInicialDeUsuario(void)
{
	puts("INGRESE EL PATH A SU VECTOR INICIAL");
	puts("SU VECTOR INICIAL DEBE SER UNA MATRIZ DE UNA SOLA COLUMNA");

	return obtenerMatrizDeUsuario();
}

int obtenerCantidadDeDecimales(void)
{
	puts("INGRESE LA CANTIDAD DE DECIMALES");
	printf("--->");
	char * decs;
	int rtdo;

	decs = getUserInput();
	rtdo = atoi(decs);
	
	free(decs);

	return rtdo;
}

float obtenerCotaDeErrorDeUsuario(void)
{
	puts("INGRESE LA COTA DE ERROR");
	printf("--->");
	char * error;
	float rtdo;

	error = getUserInput();
	rtdo = atof(error);
	
	free(error);

	return rtdo;
}

Metodo_X * preguntarSiDeseaUsarOtroMetodo(void)
{
	puts("Ud. desea resolver el mismo sistema por otro metodo");
	puts("[y/n]");
	char *  rta;
	Metodo_X * rtdo;
	rta = getUserInput();

	if( !strcmp("y", rta) )
		rtdo = obtenerMetodoDeUsuario();
	else
	if( !strcmp("n", rta) )
		rtdo = NULL;
	else
	{
		puts("Ingrese una opcion correcta");
		rtdo = preguntarSiDeseaUsarOtroMetodo();
	}

	free(rta);
	return rtdo;
}

int preguntarSiDeseaResolverOtraMatriz(void)
{
	puts("Ud. desea resolver otra matriz");
	puts("[y/n]");

	char * rta, rtdo;
	rta = getUserInput();

	if( !strcmp("y", rta) )
		rtdo = 1;
	else
	if( !strcmp("n", rta) )
		rtdo = 0;
	else
	{
		puts("Ingrese una opcion correcta");
		rtdo = preguntarSiDeseaResolverOtraMatriz();
	}

	free(rta);

	return rtdo;
}

int main(int argc, char * argv[])
{	
	Matrix * mAB, * mA, * mB, * X0;
	Dominancia dom;
	Metodo_X * metodo_X;
	float cota_error;
	int una_mas = 0, decimales;


	puts("===== SIEL =====");
	
	do
	{
		do
		{
			mAB = obtenerMatrizDeUsuario();
			getABfromMatrix(mAB, &mA, &mB);
			dom = diagonalmenteDominante(mA);
			if(!dom)
			{
				puts("Su matrix no es diagonalmente dominante");
				puts("Modifiquela hasta que lo sea");
				freeMatrix(mA);
				freeMatrix(mB);
				freeMatrix(mAB);
			}
		}		
		while(!dom);


		metodo_X = obtenerMetodoDeUsuario();
		X0 = obtenerVectorInicialDeUsuario();
		decimales = obtenerCantidadDeDecimales();
		cota_error = obtenerCotaDeErrorDeUsuario();

		do
		{
			metodo_resolver(mA, mB, X0, metodo_X, cota_error, decimales);
			metodo_X = preguntarSiDeseaUsarOtroMetodo();
		}
		while( metodo_X != NULL );

		una_mas = preguntarSiDeseaResolverOtraMatriz();
	}
	while( una_mas );

	//calcularNorma2(mAB);

	freeMatrix(mAB);
	

	return 0;
}
