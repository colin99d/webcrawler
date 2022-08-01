#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "_requests.h"
#include "_files.h"
#include "_strings.h"
#include "_prints.h"
#include "_sqlite.h"

/*
TODO: Make the url save more memory efficient (don't default to 2048 characters)
TODO: Swtich from .dat to sqlite
TODO: increase the amount of information stored (linked list?)
TODO: take arguments for starting url
*/ 

static volatile int keepRunning = 1;

int is_duplicate(struct URL * urls, int total_urls, char *string) {
	int i;

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
				end = find_next(string, '"', '\'', i+6);
				if (end == -1) {
					continue;
				}
				found = substr(string, i+6, end);
				sub_str = substr(string, i+6, i+10)	;
				if(strcmp(sub_str, "http") == 0) {
					if(is_duplicate(urls, *total_urls, found) == 1) {
						// red(found);
					} else {
						// yellow(found);
						if (strlen(found) > MAX_URL_LENGTH) {
							continue;
						}
						strcpy(urls[*total_urls].url, found);
						urls[*total_urls].scanned = 0;
						(*total_urls)++;
					}
				}
			}
		}
	}
}

void intHandler(int dummy) {
	    keepRunning = 0;
	    printf("\nSaving...\n");
}

int main() {
	static struct URL all_urls[MAX_URLS];
	static struct URL first_url;
	struct MemoryStruct * html;
	int total_urls = 0;
	int i;

	signal(SIGINT, intHandler);

	read_file(&total_urls, all_urls);
	printf("Total URLS at start: %i\n", total_urls);
	if(total_urls == 0) {
		strcpy(first_url.url, "https://www.example.com");
		first_url.scanned = 0;
		all_urls[total_urls] = first_url;
		total_urls++;
	}
	for(i = 0; i < total_urls; i++){
		if (keepRunning == 0) {
			break;
		}
		if (all_urls[i].scanned == 0) {
			// cyan(all_urls[i].url);
			html = get_url(all_urls[i].url);
			find_all(html->memory, "href=", all_urls, &total_urls);
			all_urls[i].scanned = 1;
			// green(all_urls[i].url);
			free(html);
			printf("URLs found: %i\tURLs scanned: %i\n", total_urls, i);
			write_file(total_urls, all_urls);
		}
	}
	return 0;
}
