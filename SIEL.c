#include "FileToString/fileToString.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef float ** MatrixData;

typedef struct
	{
		MatrixData data;
		int rows;
		int columns;
	} Matrix;

Matrix * fileToMatrixAB(char *path)
{
	Matrix * matrix;
	// OBTENER ARCHIVO COMPLETO COMO UN STRING
	char * fileContents = fileToString(path);

	char * token;
	// DIVIDIR EL CONTENIDO DEL ARCHIVO EN VARIAS CADENAS
	// LAS CADENAS SE OBTIENEN SEPARANDO EL CONTENIDO DEL
	// ARCHIVO POR EL CARACTER TABULACION
	token = strtok(fileContents, "\t");
	
	float * fila = malloc(128);
	int i = 0, j = 0, firstTime = 1;
	char *pos;
	MatrixData mAB = (MatrixData)malloc(128);
	matrix = (Matrix*)malloc( sizeof(Matrix) );

	while( token != NULL )
	{
		// SI LLEGUE AL FINAL DE LA FILA
		if(pos = strchr(token, '\n') )
		{
			// TOMO LA PRIMERA PARTE DEL TOKEN
			// COMO VALOR ULTIMO DE LA FILA
			*pos = '\0';
			fila[j] = atof(token);

			// GUARDO LA FILA EN EL ESPACIO JUSTO
			mAB[i] = (float*)realloc(fila, sizeof(float)*(j+1));

			// INICIALIZO LAS VARIABLES NECESARIAS
			// PARA UNA NUEVA ITERACION DE FILA
			fila = (float*)malloc(128);
			j = 0;

			// INCREMENTO EL NRO DE FILAS EN UNO
			i++;

			// TOMO LA SEGUNDA PARTE DEL TOKEN
			// COMO VALOR PRIMERO DE LA NUEVA FILA
			fila[j++] = atof(pos + 1);
		}
		// SI EL TOKEN NO ES UN CARACTER PIPE
		else if( strcmp(token, "|") )
			// TOMO EL VALOR DEL TOKEN
			// COMO VALOR DE POSICION J DE LA FILA ACTUAL
			fila[j++] = atof(token);

		// OBTENGO LA SIGUIENTE CADENA LUEGO DE LA TABULACION
		token = strtok(NULL, "\t");
	}
	// GUARDO EL NRO DE FILAS Y COLUMNAS ACUMULADO
	matrix->columns = j;
	matrix->rows = ++i;

	// GUARDO LA ULTIMA FILA EN EL ESPACIO JUSTO
	mAB[matrix->rows - 1] = realloc(fila, sizeof(float)*matrix->columns);

	// GUARDO LOS PUNTEROS A FILAS EN EL ESPACIO JUSTO
	mAB = realloc(mAB, sizeof(float*)*matrix->rows);

	// GUARDO EL VECTOR DE PUNTEROS A FILAS
	matrix->data = mAB;

	free(fileContents);

	return matrix;
}

void freeMatrix(Matrix * m)
{
	int i;

	// LIBERAR CADA FILA
	for( i=0; i < m->rows; i++)
		free(m->data[i]);

	// LIBERAR ESTRUCTURA Y SUS VALORES
	free(m);
}

void printMatrix(Matrix * m)
{
	int i, j;

	printf("Es una matriz de %d x %d\n", m->rows, m->columns);

	for(i = 0; i < m->rows; i++ )
	{
		for( j = 0; j < m->columns; j++)
			printf("%g\t", m->data[i][j]);
		printf("\n");
	}
}

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
