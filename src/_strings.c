#include <string.h>
#include <stdlib.h>


char* substr(const char *src, int m, int n) {
	int len = n - m;

	char *dest = (char*)malloc(sizeof(char) * (len + 1));

	for (int i = m; i < n && (*(src + i) != '\0'); i++)
	{
			*dest = *(src + i);
			dest++;
	}

	*dest = '\0';

	return dest - len;
}

int find_next(char *string, char target1, char target2, int start) {
	while(start < strlen(string)){
		if(string[start] == target1) {
			return start;	
		} 
		if(string[start] == target2) {
			return start;	
		} 
		start++;
	}
	return -1;
}


// This is unused, saving for the future


char in_array(char ** array, char * string) {
	while(*array != 0) {
		if (strcmp(*array, string)==0) {
			return 1;
		}
		array++;
	}
	return 0;
}
