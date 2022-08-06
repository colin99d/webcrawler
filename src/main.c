#include <stdio.h>
#include <string.h>
#include <signal.h>

#include "_requests.h"
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

typedef struct result {
	char *address;
	char *parent;
} Result;

int find_all(char *string, char *sub, Result *all_results) {
	int i = 0;
	int j = 0;
	char *sub_str;
	char found[2048];
	int end;
	int str_len;

	str_len = strlen(string);
	for(i = 0; i < str_len; i++) {
		if (str_len >= i + 10) {
		sub_str = substr(string, i, i+5);
			if(strcmp(sub_str, sub) == 0) {
				printf("--------3.1--------\n");
				end = find_next(string, '"', '\'', i+6);
				if (end == -1) {
					continue;
				}
				strcpy(found, substr(string, i+6, end));
				printf("--------3.2--------\n");
				sub_str = substr(string, i+6, i+10)	;
				printf("%s\n", sub_str);
				printf("--------3.3--------\n");
				if(strcmp(sub_str, "http") == 0) {
					if (strlen(found) > MAX_URL_LENGTH) {
						continue;
					}
					printf("--------3.4--------\n");
					all_results[j].address = malloc(sizeof(char) * (strlen(found) + 1));
					printf("--------3.5--------\n");
					strcpy(all_results[j].address, found);
					printf("--------3.5--------\n");
					j++;
				}
			}
		}
	}
	return j;
}

void add_item(sqlite3 *db, char *zErrMsg, URL *urls, int *total_urls, Result the_result) {
	int response  = sql_duplicate(db, zErrMsg, the_result.address);
	if (response == 1) {
		red(the_result.address);
	} else {
		printf("Address size: %lu\n", sizeof(the_result.address));
		yellow(the_result.address);
		urls[*total_urls].address = malloc(sizeof(char) * (strlen(the_result.address) + 1));
		strcpy(urls[*total_urls].address, the_result.address);
		urls[*total_urls].scanned = 0;
		add_url(db, zErrMsg, the_result.address, 0);
		(*total_urls)++;
	}
}

void intHandler(int dummy) {
	    keepRunning = 0;
	    printf("\nSaving...\n");
}

void getData(sqlite3 *db, char *zErrMsg, URL *all_urls, int *total_urls) {
	int response;
	int i;
	response = sqlite3_open_v2("urls.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, "unix");
	if(response) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close_v2(db);
	}
	response = create_table(db, zErrMsg);
	if(response!=SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	(*total_urls) = retrieve_urls(db, zErrMsg, all_urls);
	for (i = 0; i < *total_urls; i++) {
		printf("%s\n", all_urls[i].address);
	}
	printf("Total URLS at start: %i\n", (*total_urls));
	if(total_urls == 0) {
		all_urls[0].address = malloc(sizeof(char) * 25);
		strcpy(all_urls[0].address, "https://www.example.com");
		all_urls[0].scanned = 0;
		add_url(db, zErrMsg, all_urls[0].address, 1);
		total_urls++;
	}
}

int main() {
	MemoryStruct * html;
	sqlite3 *db = 0;
	URL *all_urls = malloc(sizeof(URL));
	Result *all_results = malloc(sizeof(Result));
	int results_number;
	char *zErrMsg = 0;
	int total_urls;
	int i;

	signal(SIGINT, intHandler);
	getData(db, zErrMsg, all_urls, &total_urls);

	for(i = 0; i < total_urls; i++){
		if (keepRunning == 0) {
			break;
		}
		if (all_urls[i].scanned == 0) {
			printf("--------1--------\n");
			cyan(all_urls[i].address);
			printf("--------2--------\n");
			html = get_url(all_urls[i].address);
			printf("--------3--------\n");
			results_number = find_all(html->memory, "href=", all_results);
			printf("--------4--------\n");
			for (i = 0; i < results_number; i++) {
				add_item(db, zErrMsg, all_urls, &total_urls, all_results[i]);
			}
			printf("--------5--------\n");
			all_urls[i].scanned = 1;
			green(all_urls[i].address);
			free(html);
			printf("URLs found: %i\tURLs scanned: %i\n", total_urls, i);
		}
	}
	sqlite3_close_v2(db);
	return 0;
}
