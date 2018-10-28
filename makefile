CC = gcc
CFLAGS = -g
VPATH = FileToString Matrix Array Matrix/Metodos
OBJS = SIEL.o fileToString.o matrix.o array.o jacobi.o

SIEL : $(OBJS)
	gcc -g -o SIEL $(OBJS)

SIEL.o : matrix.h
jacobi.o : matrix.h jacobi.h
matrix.o : matrix.h fileToString.h array.h
fileToString.o : fileToString.h
array.o : array.h

clean:
	rm SIEL
	rm *.o
