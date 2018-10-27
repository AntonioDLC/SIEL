#include "fileToString.h"

char * fileToString(const char *path)
{
	FILE *f;
	char *contents = NULL;
	int size = 0, i = 1;

	f = fopen(path, "r");
	if(!f)
		return NULL;

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	contents = (char*)malloc(size);

	fseek(f, 0, SEEK_SET);
	fread(contents, size+1, 1, f);

	if( feof(f) )
	{
		unsigned int lastPos = ftell(f);
		contents[lastPos-1] = '\0';
		fclose(f);
		return contents;
	}
	else
	{
		printf("Unable to read file from path:<%s>\n", path);
		free(contents);
		fclose(f);
		return NULL;
	}
}

