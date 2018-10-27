CC = gcc
VPATH = ../FileToString
OBJS = SIEL.o fileToString.o

SIEL : $(OBJS)
	gcc -o SIEL $(OBJS)

SIEL.o : fileToString.h
fileToString.o : fileToString.h
