#include <string.h>
#include <stdlib.h>


#include <stdio.h>
char* substr(const char *src, int m, int n) {
	int len = n - m;

	// allocate (len + 1) chars for destination (+1 for extra null character)
	printf("-----2.1-----\n");
	// printf("%s\n", src);
	char *dest = (char*)malloc(sizeof(char) * (len + 1));
	printf("-----2.2-----\n");

	// extracts characters between m'th and n'th index from source string
	for (int i = m; i < n && (*(src + i) != '\0'); i++)
	{
			*dest = *(src + i);
			dest++;
	}
	printf("-----2.3-----\n");

	*dest = '\0';

	printf("-----2.4-----\n");

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
