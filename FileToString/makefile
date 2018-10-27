test: fileToString.o test.o
	gcc -o test test.o fileToString.o

test.o: fileToString.h
fileToString.o: fileToString.h

clean:
	rm *.o
	rm test
