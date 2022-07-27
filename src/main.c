#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "requests.h"


char* substr(const char *src, int m, int n)
{
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

void check_href(char *html) {
	int i;
	char *sub_str;

	for (i = 0; i < strlen(html); i++) {
		sub_str = substr(html, i, i+5);
		if (strcmp(sub_str, "href=") == 0) {
			printf("%s\n", sub_str);
		}
	}
}


int main() {
	struct MemoryStruct * html = get_url("https://www.example.com");
	check_href(html->memory);
	free(html);
	return 0;
}
