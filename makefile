CC = gcc
CFLAGS = -g
VPATH = FileToString Matrix Array Matrix/Metodos
OBJS = SIEL.o fileToString.o matrix.o array.o metodo.o

SIEL : $(OBJS)
	gcc -g -o SIEL $(OBJS) -lm

SIEL.o : matrix.h
jacobi.o : matrix.h metodo.h
matrix.o : matrix.h fileToString.h array.h
fileToString.o : fileToString.h
array.o : array.h

clean:
	rm SIEL
	rm *.o
