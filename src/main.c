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
TODO: add logs
*/ 

static volatile int keepRunning = 1;


int is_duplicate(URL * urls, int total_urls, char *string) {
		int i;

			for(i = 0; i < total_urls; i++) {

						if(strcmp(string, urls[i].address) == 0) {
										return 1;
												}
							}
				return 0;
}

void find_all(char *string, char sub[10], URL * urls, int * total_urls, sqlite3 *db, char *zErrMsg) {
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
						red(found);
					} else {
						yellow(found);
						if (strlen(found) > MAX_URL_LENGTH) {
							continue;
						}
						strcpy(urls[*total_urls].address, found);
						strcpy(urls[*total_urls].address, found);
						urls[*total_urls].scanned = 0;
						add_url(db, zErrMsg, found, 0);
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

void getData(sqlite3 *db, char *zErrMsg, URL *all_urls, int *total_urls) {
	int response;
	int i;
	response = create_table(db, zErrMsg);
	if(response!=SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	(*total_urls) = retrieve_urls(db, zErrMsg, all_urls);
	for (i = 0; i < *total_urls; i++) {
	}
	printf("Total URLS at start: %i\n", (*total_urls));
	if(total_urls == 0) {
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
	// getData(db, zErrMsg, all_urls, &total_urls);

	for(i = 0; i < total_urls; i++){
		if (keepRunning == 0) {
			break;
		}
		if (all_urls[i].scanned == 0) {
			cyan(all_urls[i].address);
			html = get_url(all_urls[i].address);
			find_all(html->memory, "href=", all_urls, &total_urls, db, zErrMsg);
			all_urls[i].scanned = 1;
			white(all_urls[i].address);
			update_url(db, zErrMsg, all_urls[i].address);
			green(all_urls[i].address);
			free(html);
			printf("URLs found: %i\tURLs scanned: %i\n", total_urls, i);
		}
	}
	sqlite3_close_v2(db);
	return 0;
}
