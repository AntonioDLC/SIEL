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

	return sqrt(radioEspectral);
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
	/*Primero Calcular los autovalores: Pasos
1)Restarle a A X veces una matriz identidad con la misma dimension
2)Calcular el determinante de la matriz resultante
3)Calcular los raices de la ecuacion resultante
*/

	//Luego obtener el maximo y retornarlo
	return 0;
}

double calcularNorma1(Matrix * m){
	double maximo = 0,sum;
		int i, j;

	for( i = 0; i < m->columns; i++)
	{
		sum = 0;
		for( j = 0; j < m->rows; j++)
			sum += fabs(m->data[i][j]);

		if( sum > maximo )	maximo = sum;
	}
	return maximo;
}

float determinanteCuadradas(Matrix * mat)
{
	//n: orden de la matrix
int i,j,k,l = 0,n,m;
float det;
n = mat->rows;
m = n -1;
det=mat->data[1][1]; 
for(k=1;k<=m;k++) 
{ l=k+1; 
for(i=l;i<=n;i++) 
{ for(j=l;j<=n;j++) 
mat->data[i][j] = ( mat->data[k][k]*mat->data[i][j]-mat->data[k][j]*mat->data[i][k] )/mat->data[k][k]; } 
det=det*mat->data[k+1][k+1]; 
}
return det;
}


/*
float determinanteCuadradas(Matrix *mat, int orden){

        float determinante = 0, aux = 0;

        int c;
		Matrix menor[mat->rows-1];

        if(mat->rows == 2)
                return mat->data[0][0]*mat->data[1][1] - mat->data[1][0]*mat->data[0][1];

        else{

                for(int j=0; j< mat->rows; j++){

						//Matrix **menor = matrix_new(orden-1,orden-1);
                        //float **menor = (float **)malloc(sizeof(float)*(orden -1));

                        for(int h=0; h<(orden-1); h++) menor[h] = matrix_new(orden-1,orden-1);;

                        for(int k=1; k<orden; k++){

                                c = 0;

                                for(int l=0; l<orden; l++){

                                        if(l!=j){

                                                menor->data[k-1][c] = mat->data[k][l];

                                                c++;

                                        }

                                }

                        }

                        aux = pow(-1, 2+j)*mat->data[0][j]*determinanteCuadradas(menor, orden-1);

                        determinante += aux;					

                }

                return determinante;

		}

    }
*/
/*
	double * autovalores(Matrix * m) {
int i, j, ip, iq, nrot, n;
 
   n = m->rows;
 
   double **v, **f;
 
   //v = new double *[n];
   v = malloc(sizeof(double)*n);
   //f = new double *[n];
   f  = malloc(sizeof(double)*n);
 
   for(i = 0; i< m->rows; i++){
 
                v[i] = malloc(sizeof(double)*n);
                f[i] = malloc(sizeof(double)*n);
 
        }
 
        for(i=0; i < n; i++){
 
                for(j=0; j < n; j++){
 
                        f[i][j] = m->data[i][j];
               
                }
       
        }
 
// Define las variables de tipo doble
 
   double *b, *z, *d;
 
   b = malloc(sizeof(double)*n);
   z = malloc(sizeof(double)*n);
   d = malloc(sizeof(double)*n);
 
   double c, g, h, s, sm, t, tau, theta, tresh;
 
// Inicializa a la matriz identidad
 
   for (ip = 0; ip < n; ip++) {
 
      for (iq = 0; iq < n; iq++) {
 
         v[ip][iq] = 0;
 
      }
 
      v[ip][ip] = 1;
 
   }
 
// Inicializa b y d a la diagonal de a
 
   for (ip = 0; ip < n; ip++) {  
 
      b[ip] = f[ip][ip];
 
      d[ip] = b[ip];
 
      z[ip] = 0;
 
   }
 
   nrot = 0;
 
   for (i = 0; i < 50; i++) {
 
      sm = 0;
 
      for (ip = 0; ip < n - 1; ip++) {
 
         for (iq = ip + 1; iq < n; iq++) {
 
            sm +=fabs(f[ip][iq]);
 
         }
 
      }
 
      if (sm == 0) break;
 
      if (i < 4)
 
         tresh = 0.2*sm/(n*n);
 
      else
 
         tresh = 0.0;
 
      for (ip =0; ip < n -1; ip++) {
 
         for (iq = ip + 1; iq < n; iq++) {
 
            g = 100.0*fabs(f[ip][iq]);
 
            if(i>4 && (double)(fabs(d[ip])+g) == (double)fabs(d[ip])
 
               && (double)(fabs(d[iq])+g) == (double)fabs(d[iq]))
 
               f[ip][iq] = 0.0;
 
            else if (fabs(f[ip][iq]) > tresh) {
 
               h = d[iq] - d[ip];
 
               if ((double)(fabs(h)+g) == (double)fabs(h))
 
                  t = (f[ip][iq])/h;   // t = 1/(2theta)
 
               else {
 
                  theta = 0.5*h/(f[ip][iq]);
 
                  t = 1.0/(fabs(theta)+sqrt(1.0+theta*theta));
 
                  if(theta < 0.0) t = -t;
 
               }
 
                  c = 1.0/sqrt(1+t*t);
 
                  s = t*c;
 
                  tau = s/(1.0+c);
 
                  h = t*f[ip][iq];
 
                  z[ip] -=h;
 
                  z[iq] +=h;
 
                  d[ip] -=h;
 
                  d[iq] +=h;
 
                  f[ip][iq] = 0.0;
 
// Varía desde 0 hasta  ip - 1
 
               for (j =0; j < ip; j++) {
 
                  g = f[j][ip];
 
                  h = f[j][iq];
 
                  f[j][ip] = g - s*(h+g*tau);
 
                  f[j][iq] = h + s*(g-h*tau);
 
               }
 
// Varía desde ip+1 hasta  iq - 1
 
               for (j =ip+1; j < iq; j++) {
 
                  g = f[ip][j];
 
                  h = f[j][iq];
 
                  f[ip][j] = g - s*(h+g*tau);
 
                  f[j][iq] = h + s*(g-h*tau);
 
               }
 
               for (j =iq+1; j < n; j++) {
 
                  g = f[ip][j];
 
                  h = f[iq][j];
 
                  f[ip][j] = g - s*(h+g*tau);
 
                  f[iq][j] = h + s*(g-h*tau);
 
               }
 
               for (j =0; j < n; j++) {
 
                  g = v[j][ip];
 
                  h = v[j][iq];
 
                  v[j][ip] = g - s*(h+g*tau);
 
                  v[j][iq] = h + s*(g-h*tau);
 
                  }
 
               ++(nrot);
 
            }
 
         }
 
      }
 
         for (ip = 0; ip < n; ip++) {
 
            b[ip] = b[ip]+z[ip];
 
            d[ip] = b[ip];
 
            z[ip] = 0.0;
 
         }
 
        for(i=0; i < n; i++){
 
                for(j=0; j < n; j++){
               
                        if(i != j) m->data[i][j] = 0;
 
                                else m->data[i][i] = d[i];
               
                }
       
        }
 
		return NULL;
   }  
 

}
*/

