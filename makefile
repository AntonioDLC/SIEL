CC = gcc
CFLAGS = 
VPATH = FileToString Matrix
OBJS = SIEL.o fileToString.o matrix.o

SIEL : $(OBJS)
	gcc -o SIEL $(OBJS)

SIEL.o : matrix.h
matrix.o : matrix.h fileToString.h
fileToString.o : fileToString.h

clean:
	rm SIEL
	rm *.o
