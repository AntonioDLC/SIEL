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
	fflush(stdin);
	return userInput;
}

Matrix * obtenerMatrizDeUsuario()
{
	printf(
	"\nINGRESE EL PATH A SU MATRIZ AQUI --->"
	);

	char * path = NULL;
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

	//puts("Genial.");
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


void menu(void)
{
	Matrix * mAB = NULL, * mA, * mB, * X0;
	Dominancia dom;
	Metodo_X * metodo_X;
	float cota_error;
	int una_mas = 0, decimales;
	int operacion=0;
	float norma;	
	mAB = obtenerMatrizDeUsuario();
	getABfromMatrix(mAB, &mA, &mB);
		
	puts("Ingrese el numero de operacion a realizar:");	

	puts("1-Seleccionar un método para resolver el sistema");
	puts("2-Calcular norma 1");
	puts("3-Calcular norma 2");
	puts("4-Calcular norma infinito");
	puts("5-Salir");

	scanf("%i", &operacion);
	fflush(stdin);

	switch(operacion)
	{
		case 1:
		dom = diagonalmenteDominante(mA);
		while(!dom)
		{
			freeMatrix(mA);
			freeMatrix(mB);
			freeMatrix(mAB);
				puts("Su matrix no es diagonalmente dominante");
				puts("Modifiquela hasta que lo sea");
				
			mAB = obtenerMatrizDeUsuario();
			getABfromMatrix(mAB, &mA, &mB);
			dom = diagonalmenteDominante(mA);
		}
		metodo_X = obtenerMetodoDeUsuario();
		X0 = obtenerVectorInicialDeUsuario();
		decimales = obtenerCantidadDeDecimales();
		cota_error = obtenerCotaDeErrorDeUsuario();
		metodo_resolver(mA, mB, X0, metodo_X, cota_error, decimales);	
		break;
		case 2:
		norma = calcularNorma1(mA);
		printf("Resultado: %f\n",norma);
		break;
		case 3:
		norma = calcularNorma2(mA);
		printf("Resultado: %f\n",norma);
		break;
		case 4:
		norma = calcularNormaInf(mA);
		printf("Resultado: %f\n",norma);
		break;
		case 5:
						puts("Saliendo de SIEL");
						exit(-1);
		break;
		default:
		puts("Operacion no valida. Ingrese una operacion valida");
	}

	freeMatrix(mAB);
	
	puts("Para realizar otra operacion ingrese 1, de lo contrario presione 2");

	scanf("%i", &operacion);
	fflush(stdin);

	if(operacion != 1)
	{
		puts("Saliendo de SIEL");
		exit(-1);
	}
	menu();
}



int main(int argc, char * argv[])
{	
	puts("===== SIEL =====");
	menu();
	return 0;
}

/*
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
*/
