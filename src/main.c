#include <stdio.h>
#include <string.h>

#include "_requests.h"
#include "_files.h"
#include "_strings.h"


void find_all(char *string, char sub[10]) {
	int i;
	char *sub_str;
	char *found;
	int end;

	for (i = 0; i < strlen(string); i++) {
		sub_str = substr(string, i, i+5);
		if (strcmp(sub_str, sub) == 0) {
			end = find_next(string, '"', i+6);
			found = substr(string, i+6, end);
			printf("%s\n", found);
		}
	}
}



int main() {
	static struct URL all_urls[MAX_URLS];
	static struct URL first_url;
	struct MemoryStruct * html;
	int total_urls = 0;

	read_file(&total_urls, all_urls);
	printf("Total URLS at start: %i\n", total_urls);
	if (total_urls == 0) {
		strcpy(first_url.url, "https://www.example.com");
		first_url.scanned = 0;
		all_urls[total_urls] = first_url;
		total_urls++;
		printf("Added url\n");
	}
	html = get_url(all_urls[0].url);
	// find_all(html->memory, "href=");
	// free(html);
	write_file(total_urls, all_urls);
	printf("Total URLS at end: %i\n", total_urls);
	return 0;
}
