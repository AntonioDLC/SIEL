#include "fileToString.h"

int main(int argc, char *argv[])
{
	char *fileContents;
	fileContents = fileToString(argv[1]);

	printf(fileContents);

	free(fileContents);

	return 0;
}
