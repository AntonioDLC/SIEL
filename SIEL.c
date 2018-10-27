#include "../FileToString/fileToString.h"
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
	char * fileContents = fileToString(path);
	puts(fileContents);

	char * token;
	token = strtok(fileContents, "\t");
	
	float * fila = malloc(128);
	int i = 0, j;
	char *pos;
	MatrixData mAB = malloc(128);
	for( j = 0; token != NULL; )
	{
		printf("<%s>\n",token);
		if(pos = strchr(token, '\n') )
		{
			*pos = '\0';
			fila[j] = atof(token);
			printf("fil : %g\n", fila[j]);
			mAB[i] = realloc(fila, sizeof(float)*(j+1));
			fila = malloc(128);
			j = 0;
			i++;
			fila[j++] = atof(pos + 1);
		}
		else if( strcmp(token, "|") )
			fila[j++] = atof(token);

		printf("fil : %g\n", fila[j-1]);
		token = strtok(NULL, "\t");
	}

	mAB[i] = realloc(fila, sizeof(float)*j);
	i++;

	mAB = realloc(mAB, sizeof(float*)*i);

	matrix = malloc( sizeof(Matrix) );
	matrix->rows = i;
	matrix->columns = j;
	matrix->data = mAB;
	return matrix;
}
	
void printMatrix(Matrix * m)
{
	int i, j;

	printf("It is a %d x %d matrix\n", m->rows, m->columns);

	for(i = 0; i < m->rows; i++ )
	{
		for( j = 0; j < m->columns; j++)
			printf("%g\t", m->data[i][j]);
		printf("\n");
	}
}

int main(void)
{
	Matrix * mAB;

	mAB = fileToMatrixAB("matrizAB.txt");
	printMatrix(mAB);

	return 0;
}
