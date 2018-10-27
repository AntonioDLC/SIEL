CC = gcc
CFLAGS = 
VPATH = FileToString
OBJS = SIEL.o fileToString.o

SIEL : $(OBJS)
	gcc -o SIEL $(OBJS)

SIEL.o : fileToString.h
fileToString.o : fileToString.h

clean:
	rm *.o
