#include <stdlib.h>

struct MemoryStruct {
	char *memory;
	size_t size;
};

struct MemoryStruct * get_url(char url[1000]); 
