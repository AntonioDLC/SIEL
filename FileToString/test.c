#include "fileToString.h"
#include <string.h>

int main(int argc, char *argv[])
{
	if( argc != 2 )
	{
		puts("Falta un parametro");
		return -1;
	}

	char *fileContents;
	fileContents = fileToString(argv[1]);

	printf("Size = %d\n", strlen(fileContents));
	puts(fileContents);

	free(fileContents);

	return 0;
}
