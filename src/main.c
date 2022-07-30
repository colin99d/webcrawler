#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "requests.h"

#define URL_FILE "urls.dat"
#define MAX_URLS 10000
#define MAX_URL_LENGTH 2048

struct URL {
	char url[MAX_URL_LENGTH];
	int scanned;
};

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

void write_file(int total_urls, struct URL *urls) {
	FILE *outfile = fopen(URL_FILE, "wb");
	int i;

	for (i=0; i<total_urls; i++) {
		fwrite(&urls[i], sizeof(struct URL), 1, outfile);
	}
	fclose(outfile);
}

void read_file(int *url_count, struct URL *urls) {
	FILE *infile = fopen(URL_FILE, "rb");
	struct URL input;
	int i = 0;


	if(infile == NULL) {
			fprintf(stderr, "\nError opened file\n");
	} else {
		while(fread(&input, sizeof(struct URL), 1, infile)) {
			urls[i].scanned = input.scanned;
			strcpy(urls[i].url, input.url);
			(*url_count)++;
		}
	}
	fclose(infile);
}


int main() {
	static struct URL all_urls[MAX_URLS];
	static struct URL first_url;
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
	// struct MemoryStruct * html = get_url();
	// find_all(html->memory, "href=");
	// free(html);
	write_file(total_urls, all_urls);
	printf("Total URLS at end: %i\n", total_urls);
	return 0;
}
