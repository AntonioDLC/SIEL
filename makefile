CC = gcc
CFLAGS = -g
VPATH = FileToString Matrix Array
OBJS = SIEL.o fileToString.o matrix.o array.o

SIEL : $(OBJS)
	gcc -g -o SIEL $(OBJS)

SIEL.o : matrix.h
matrix.o : matrix.h fileToString.h array.h
fileToString.o : fileToString.h
array.o : array.h

clean:
	rm SIEL
	rm *.o
