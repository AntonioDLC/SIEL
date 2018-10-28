#include "../Array/array.h"
#include "matrix.h"
#include "../FileToString/fileToString.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

int elem_esDominante( Matrix * m, int fila, int col)
{
	int i, sum = 0, elem = m->data[fila][col];

	for( i = 0; i < m->columns; i++ )
		if( col != i )
			sum += fabsf(m->data[fila][i]);

	if( elem >= sum )
	{
		if( elem > sum )
			return ESTRICTA;
		else
			return SIMPLE;
	}
	else
		return NO_DOMINANTE;
}
			

int fila_getDominante( Matrix * m, int fila, float * dom, float * col)
{
	int i, tipoDom;
	for(i = 0; i < m->columns; i++)
	{
		if( tipoDom = elem_esDominante(m, fila, i) )
		{
			*dom = m->data[fila][i];
			*col = i;
			return tipoDom;
		}
	}

	*dom = -1;
	*col = -1;

	return NO_DOMINANTE;
}

int diagonalmenteDominante(Matrix * m)
{
	float escalarDom, nroColumna;
	Dominancia tipoDomFila, tipoDomMatriz = ESTRICTA;
	Array * columnasDominantes = array_new(sizeof(int));

	int i;
	for( i = 0; i < m->rows; i++ )
	{
		tipoDomFila = fila_getDominante(m, i, &escalarDom, &nroColumna);

		if(!array_contains(columnasDominantes, &nroColumna))
		{
			array_add(columnasDominantes, &nroColumna);
			if(tipoDomFila == SIMPLE)
				tipoDomMatriz = SIMPLE;
		}
		else
		{
			tipoDomMatriz = NO_DOMINANTE;
			break;
		}
	}
	array_free(columnasDominantes);

	return tipoDomMatriz;
}

Matrix * matrix_subcopy(Matrix * m, int xi, int yi, int dx, int dy)
{
	Matrix * subcopy = calloc(1, sizeof(Matrix));

	subcopy->rows = dy;
	subcopy->columns = dx;

	subcopy->data = (MatrixData)malloc( subcopy->rows*sizeof(float*) );
	int i;
	for( i = 0; i < subcopy->rows; i++)
		subcopy->data[i]=(float*)malloc(subcopy->columns*sizeof(float));

	int j;
	for( i = 0; i < subcopy->rows; i++)
		for( j = 0; j < subcopy->columns; j++)
			subcopy->data[i][j] = m->data[yi + i][xi + j];

	return subcopy;
}

int isMatrixAB(Matrix * m)
{
	return m->rows == m->columns - 1;
}

void getABfromMatrix(Matrix * m, Matrix ** a, Matrix ** b)
{
	if( !isMatrixAB(m) )
	{
		puts("La matriz no puede descomponerse en las de un SEL");
		return;
	}
	*a = matrix_subcopy(m, 0, 0, m->rows, m->rows);
	*b = matrix_subcopy(m, m->columns-1, 0, 1, m->rows);
}

Matrix * fileToMatrix(char *path)
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
