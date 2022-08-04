#include <stdlib.h>

typedef struct memorystruct {
	char *memory;
	size_t size;
} MemoryStruct;

MemoryStruct * get_url(char url[1000]); 
