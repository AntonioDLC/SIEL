#include "../Array/array.h"
#include "matrix.h"
#include "../FileToString/fileToString.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

Matrix * matrix_new( int rows, int columns )
{
	Matrix * m = malloc( sizeof(Matrix) );
	m->rows = rows;
	m->columns = columns;

	m->data = calloc( m->rows, sizeof(float*) );

	int i;
	for( i = 0; i < m->rows; i++)
		m->data[i] = calloc( m->columns, sizeof(float) );

	return m;
}

Matrix * matrix_div_esc( Matrix * m, float esc)
{
	Matrix * rtdo = matrix_new( m->rows, m->columns);

	int i,j;
	for(i=0; i < m->rows; i++)
		for(j=0; j < m->columns; j++)
			rtdo->data[i][j] = m->data[i][j]/esc;

	return rtdo;
}

Matrix * matrix_mult_esc( Matrix * m, float esc)
{
	Matrix * rtdo = matrix_new( m->rows, m->columns);

	int i,j;
	for(i=0; i < m->rows; i++)
		for(j=0; j < m->columns; j++)
			rtdo->data[i][j] = m->data[i][j]*esc;

	return rtdo;
}

Matrix * matrix_sum( Matrix * m1, Matrix * m2 )
{
	if( m1->columns != m2->columns || m1->rows != m2->rows )
		return NULL;

	Matrix * rtdo;
	rtdo = matrix_new( m1->rows, m1->columns );

	int i, j;
	for( i = 0; i < rtdo->rows; i++ )
		for( j = 0; j < rtdo->columns; j++ )
			rtdo->data[i][j] = m1->data[i][j] + m2->data[i][j];
	
	return rtdo;
}

Matrix * matrix_mult( Matrix * m1, Matrix * m2 )
{
	if( m1->columns != m2->rows )
		return NULL;

	Matrix * rtdo;
	rtdo = matrix_new( m1->rows, m2->columns );

	int i, j, k;
	for( i = 0; i < rtdo->rows; i++ )
		for( j = 0; j < rtdo->columns; j++ )
		{
			float sum = 0;
			for( k = 0; k < m1->columns; k++ )
				sum += m1->data[i][k]*m2->data[k][j];

			rtdo->data[i][j] = sum;
		}
/* ONLY UNCOMMENT TO DEBUG THIS FUNCION
	puts("------------------");

	printMatrix(m1);

	puts("MULTIPLICADA CON");

	printMatrix(m2);

	puts("DA COMO RESULTADO");

	printMatrix(rtdo);

	puts("------------------");
*/
	return rtdo;
}

int elem_esDominante( Matrix * m, int fila, int col)
{
	int i, sum = 0, elem = m->data[fila][col];

	for( i = 0; i < m->columns; i++ )
		if( col != i )
			sum += fabsf(m->data[fila][i]);

	if( fabsf(elem) >= sum )
	{
		if( fabsf(elem) > sum )
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
	Dominancia tipoDomFila;

	int i;
	for( i = 0; i < m->rows; i++ )
	{
		tipoDomFila = elem_esDominante(m, i, i);
		if( tipoDomFila < ESTRICTA)
			break;
	}

	for( ; i < m->rows; i++ )
	{
		tipoDomFila = elem_esDominante(m, i, i);
		if( tipoDomFila < SIMPLE)
			break;
	}

	return tipoDomFila;
}


int puedeSerDiagonalmenteDominante(Matrix * m)
{
	float escalarDom, nroColumna;
	Dominancia tipoDomFila, tipoDomMatriz = ESTRICTA;
	Array * columnasDominantes = array_new(sizeof(int));

	int i;
	for( i = 0; i < m->rows; i++ )
	{
		tipoDomFila = fila_getDominante(m, i, &escalarDom, &nroColumna);

		if(tipoDomFila && !array_contains(columnasDominantes, &nroColumna))
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

Matrix * matrix_copy(Matrix * m)
{
	return matrix_subcopy(m, 0, 0, m->columns, m->rows);
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

	if(!fileContents)
		return NULL;

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
			do
			{
				// TOMO LA PRIMERA PARTE DEL TOKEN
				// COMO VALOR ULTIMO DE LA FILA
				*pos = '\0';
				fila[j] = atof(token);
	
				// GUARDO LA FILA EN EL ESPACIO JUSTO
				mAB[i] = 
				(float*)realloc(fila, sizeof(float)*(j+1));
	
				// INICIALIZO LAS VARIABLES NECESARIAS
				// PARA UNA NUEVA ITERACION DE FILA
				fila = (float*)malloc(128);
				j = 0;
	
				// INCREMENTO EL NRO DE FILAS EN UNO
				i++;
				token = pos + 1;
			}
			while(pos = strchr(token, '\n') );

			fila[j++] = atof(token);
		}

		// SI EL TOKEN NO ES UN CARACTER PIPE
		else if( !pos && strcmp(token, "|") )
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

//	printf("Una matriz de %d x %d\n", m->rows, m->columns);

	for(i = 0; i < m->rows; i++ )
	{
		for( j = 0; j < m->columns; j++)
			printf("%g\t", m->data[i][j]);
		putchar('\n');
	}
	putchar('\b');
}

void printVector(Matrix * m, int cant_decimales)
{
	int i;

//	printf("Vector de dimension %d:\n", m->rows);
	char * formato = malloc(strlen(" %.3f,")+1);
	sprintf( formato, " %%.%df,", cant_decimales);
	printf("(");
	for( i = 0; i < m->rows; i++)
		printf(formato, m->data[i][0]);

	printf("\b )");
}

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
//Proced: Obtener la m traspuesta mt, multiplicar m y mt, obtener el radioEsp, hacer raiz(radioEsp)
double calcularNorma2(Matrix * m)
{
	Matrix * nula = NULL; //Ver de eliminarla en lo posible
	Matrix * c;// = matrix_new(m->rows,m->columns - 1);

	getABfromMatrix(m,&c,&nula);
	freeMatrix(nula);

	printMatrix(c);

	//Se añade el caso para que solo pueda ser cuadrada la matriz.
	if(c->rows != c->columns)
	{
		printf("Error no se puede calcular la norma 2 de una matriz no cuadrada");
		return -1;
	}

	Matrix * traspuesta;// = matrix_new(c->columns,c->rows);
	traspuesta = getTraspuesta(c);
	
	printMatrix(traspuesta);

	Matrix * producto;// = matrix_new(traspuesta->rows,c->columns);
	producto = matrix_mult(traspuesta,c);
	freeMatrix(traspuesta);
	freeMatrix(c);

	printMatrix(producto);

	float radioEspectral = obtenerRadioEspectral(producto);

	return 0; // sqrt(radioEspectral);
}

float calcularNormaInf(Matrix * m)
{
	int i, j;
	float sum, max = 0;

	for( i = 0; i < m->rows; i++)
	{
		sum = 0;
		for( j = 0; j < m->columns; j++)
			sum += fabs(m->data[i][j]);

		if( sum > max )	max = sum;
	}

	return max;
}

Matrix * getTraspuesta(Matrix * m)
{
	//Invierte la matriz
	Matrix * traspuesta = matrix_new(m->columns,m->rows);

	int i,j;
	for( i = 0; i < m->rows; i++)
		for( j = 0; j < m->columns; j++)
			traspuesta->data[j][i] = m->data[i][j];

	return traspuesta;
	
}

float obtenerRadioEspectral(Matrix * m)
{
	//Primero Calcular los autovalores

	//Luego obtener el maximo y retornarlo
	return 0;
}
