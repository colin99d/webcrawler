#include <string.h>
#include <stdlib.h>

char* substr(const char *src, int m, int n) {
	int len = n - m;

	// allocate (len + 1) chars for destination (+1 for extra null character)
	char *dest = (char*)malloc(sizeof(char) * (len + 1));

	// extracts characters between m'th and n'th index from source string
	for (int i = m; i < n && (*(src + i) != '\0'); i++)
	{
			*dest = *(src + i);
			dest++;
	}

	*dest = '\0';

	return dest - len;
}

int find_next(char *string, char target, int start) {
	while(start < strlen(string)){
		if(string[start] == target) {
			return start;	
		} else {
			start++;
		}
	}
	return -1;
}
