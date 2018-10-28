#include <stdlib.h>
#include <string.h>
#include "array.h"

Array * array_new( int elemSize)
{
	Array * arr = malloc( sizeof(Array) );
	arr->data = NULL;
	arr->totalElems = 0;
	arr->elemSize = elemSize;

	return arr;
}

void array_add( Array * arr, void * elem)
{
	int prevByteSize = arr->totalElems*arr->elemSize;
	arr->totalElems++;
	arr->data = realloc(arr->data, prevByteSize + arr->elemSize);
	memcpy(arr->data + prevByteSize, elem, arr->elemSize);
}

int array_elemcmp( Array * arr, int pos, void * elem)
{
	return memcmp(arr->data + arr->elemSize*pos, elem, arr->elemSize);
}

int array_contains( Array * arr, void * elem)
{
	int i;
	for( i = 0; i < arr->totalElems; i++ )
	{
		if(!array_elemcmp(arr, i, elem))
			return 1;
	}

	return 0;
}

void array_free( Array * arr )
{
	free(arr->data);
	free(arr);
}
