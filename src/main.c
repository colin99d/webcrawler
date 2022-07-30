#include <stdio.h>
#include <string.h>

#include "_requests.h"
#include "_files.h"
#include "_strings.h"
#include "_prints.h"

/*
TODO: Make the url save more memory efficient (don't default to 2048 characters)
TODO: Mark urls as complete once they have been searched
TODO: Exit safely on cntrl-c
*/ 

int is_duplicate(struct URL * urls, int total_urls, char *string) {
	int i;

	white(string);
	for(i = 0; i < total_urls; i++) {

		if(strcmp(string, urls[i].url) == 0) {
			return 1;
		}
	}
	return 0;
}

void find_all(char *string, char sub[10], struct URL * urls, int * total_urls) {
	int i;
	char *sub_str;
	char *found;
	int end;
	int str_len;


	str_len = strlen(string);
	for(i = 0; i < str_len; i++) {
		if (str_len >= i + 10) {
		sub_str = substr(string, i, i+5);
			if(strcmp(sub_str, sub) == 0) {
				printf("-----1-----\n");
				end = find_next(string, '"', i+6);
				printf("-----2-----\n");
				found = substr(string, i+6, end);
				printf("-----3-----\n");
				printf("%s\n", found);
				sub_str = substr(string, i+6, i+10)	;
				if(strcmp(sub_str, "http") == 0) {
					printf("-----4-----\n");
					if(is_duplicate(urls, *total_urls, found) == 1) {
						red(found);
					} else {
						yellow(found);
						strcpy(urls[*total_urls].url, found);
						urls[*total_urls].scanned = 0;
						(*total_urls)++;
					}
				}
			}
		}
	}
}


int main() {
	static struct URL all_urls[MAX_URLS];
	static struct URL first_url;
	struct MemoryStruct * html;
	int total_urls = 0;
	int i;

	read_file(&total_urls, all_urls);
	printf("Total URLS at start: %i\n", total_urls);
	if(total_urls == 0) {
		strcpy(first_url.url, "https://www.example.com");
		first_url.scanned = 0;
		all_urls[total_urls] = first_url;
		total_urls++;
	}
	for(i = 0; i < total_urls; i++){
		if (all_urls[i].scanned == 0) {
			cyan(all_urls[i].url);
			// html = get_url(all_urls[i].url);
			html = get_url("https://www.icann.org/en/public-comment?sort-param=upcoming-date&amp;sort-direction=asc&amp;upcoming-sort-param=upcoming-date&amp;upcoming-sort-direction=desc&amp;open-sort-param=open-closed-date&amp;open-sort-direction=asc&amp;pending-sort-param=pending-report-due-date&amp;pending-sort-direction=desc");
			printf("%s\n", html->memory);
			find_all(html->memory, "href=", all_urls, &total_urls);
			all_urls[i].scanned = 1;
			green(all_urls[i].url);
			// free(html);
		}
	}
	write_file(total_urls, all_urls);
	printf("Total URLS at end: %i\n", total_urls);
	return 0;
}
