typedef struct
	{
		void * data;
		int elemSize;
		int totalElems;
	} Array;

Array * array_new( int elemSize);
void array_add( Array * arr, void * elem);
int array_elemcmp( Array * arr, int pos, void * elem);
int array_contains( Array * arr, void * elem);
void array_free( Array * arr);
