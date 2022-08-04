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


int find_all(char *string, char *sub, char **result ) {
	int i = 0;
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
					if (strlen(found) > MAX_URL_LENGTH) {
						continue;
					}
					realloc(result, sizeof(char *) * (i+1));
					result[i] = malloc(sizeof(char) * (strlen(found) + 1));
					strcpy(result[i], found);
					i++;
				}
			}
		}
	}
	return i;
}

void add_item(sqlite3 *db, char *zErrMsg, URL *urls, int *total_urls, char *found) {
	if (sql_duplicate(db, zErrMsg, found) == 1) {
		red(found);
	} else {
		yellow(found);
		strcpy(urls[*total_urls].address, found);
		urls[*total_urls].scanned = 0;
		add_url(db, zErrMsg, found, 0);
		(*total_urls)++;
	}
}

void intHandler(int dummy) {
	    keepRunning = 0;
	    printf("\nSaving...\n");
}

int main() {
	MemoryStruct * html;
	sqlite3 *db = 0;
	URL *all_urls = 0;
	char **find_results = 0;
	int results_number;
	int response;
	char *zErrMsg = 0;
	int total_urls = 0;
	int i;

	signal(SIGINT, intHandler);

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

	total_urls = retrieve_urls(db, zErrMsg, all_urls);
	printf("Total URLS at start: %i\n", total_urls);
	if(total_urls == 0) {
		strcpy(all_urls[0].address, "https://www.example.com");
		all_urls[0].scanned = 0;
		add_url(db, zErrMsg, all_urls[0].address, 1);
		total_urls++;
	}
	for(i = 0; i < total_urls; i++){
		if (keepRunning == 0) {
			break;
		}
		if (all_urls[i].scanned == 0) {
			cyan(all_urls[i].address);
			html = get_url(all_urls[i].address);
			results_number = find_all(html->memory, "href=", find_results);
			for (i = 0; i < results_number; i++) {
				add_item(db, zErrMsg, all_urls, &total_urls, find_results[i]);
			}
			all_urls[i].scanned = 1;
			green(all_urls[i].address);
			free(html);
			printf("URLs found: %i\tURLs scanned: %i\n", total_urls, i);
		}
	}
	response = sqlite3_close_v2(db);
	return 0;
}
