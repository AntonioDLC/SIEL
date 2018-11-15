#ifndef _MATRIX_H_
#define _MATRIX_H_

typedef float ** MatrixData;

typedef struct
	{
		MatrixData data;
		int rows;
		int columns;
	} Matrix;

typedef enum{ NO_DOMINANTE, SIMPLE, ESTRICTA} Dominancia;

Matrix * matrix_new(int r, int c);
Matrix * fileToMatrix(char * path);
Matrix * matrix_mult_esc( Matrix * m, float esc);
Matrix * matrix_sum( Matrix *, Matrix *);
Matrix * matrix_mult(Matrix *, Matrix *);
void getABfromMatrix(Matrix * m, Matrix ** a, Matrix ** b);
void printMatrix(Matrix * m);
void printVector(Matrix * m, int cant_dec);
void freeMatrix(Matrix * m);
int podriaSerDiagonalmenteDominante(Matrix * m);
int diagonalmenteDominante(Matrix * m);
Matrix * matrix_copy(Matrix * m);
void printDom(Matrix * m);
float calcularNormaInf(Matrix * m);

//Norma 2
double calcularNorma2(Matrix * m);
double calcularNorma1(Matrix * m);
Matrix * getTraspuesta(Matrix * m);
//Hacer producto con matrix_mult()
float obtenerRadioEspectral(Matrix * m);
float determinanteCuadradas(Matrix * mat);
Matrix * autovalores(Matrix * ma);
//float determinanteCuadradas(Matrix *mat, int orden);
//Habra metodos aux para obtener lo necesario para los autovalores 

#endif
