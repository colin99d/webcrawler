#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
TODO: change hardcoded lengths to the defines
*/
#include "_sqlite.h"


static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}


int sql_table_duplicate(sqlite3 *db, char *zErrMsg, char *table) {
	sqlite3_stmt * statement;
	int response;
	char query[] =  "SELECT name FROM sqlite_schema WHERE type = 'table' AND name NOT LIKE 'sqlite_%';";

	response = sqlite3_prepare_v2(db, query, strlen(query), &statement, NULL);
	if(response != SQLITE_OK) {
		sqlite3_close(db);
		printf("SQL TABLE DUPLICATE: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(statement);
		return -1;
	}
	while (sqlite3_step(statement) == SQLITE_ROW) {	
		sqlite3_finalize(statement);
		return 1;
	}

	sqlite3_finalize(statement);
	return 0;
}

int create_table(sqlite3 *db, char *zErrMsg) {
	char stmt[] = "CREATE TABLE IF NOT EXISTS url( \
								id INT PRIMARY KEY NOT NULL UNIQUE, \
								address TEXT NOT NULL UNIQUE, \
								scanned INT NOT NULL \
								);";
	return sqlite3_exec(db, stmt, callback, 0, &zErrMsg);
}


int drop_table(sqlite3 *db, char *zErrMsg) {
	return sqlite3_exec(db, "DROP TABLE IF EXISTS url", callback, 0, &zErrMsg);
}


int add_url(sqlite3 *db, char *zErrMsg, char *url, int first) {
	char id_str[30];
	char complete_str[2200];
	if (first == 1){
		strcpy(id_str, "1");	
	} else {
		strcpy(id_str, "(SELECT max(id) FROM url)+1");
	}
	snprintf(complete_str, 2200,  "INSERT INTO url VALUES(%s,'%s', 0);", id_str, url);

	return sqlite3_exec(db, complete_str, callback, 0, &zErrMsg);
}


int sql_duplicate(sqlite3 *db, char *zErrMsg, char* str) {
	printf("%s\n", str);
	sqlite3_stmt * statement;
	int response;
	char query[2100];
	snprintf(query, 2100, "SELECT id FROM url WHERE address IS '%s';", str);
	response = sqlite3_prepare_v2(db, query, strlen(query), &statement, NULL);
	if(response != SQLITE_OK) {
		sqlite3_close(db);
		printf("SQL DUPLICATE: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(statement);
		exit(1);
		return -1;
	}
	while (sqlite3_step(statement) == SQLITE_ROW) {	
		sqlite3_finalize(statement);
		return 1;	
	}
	sqlite3_finalize(statement);
	return 0;
}


int update_url(sqlite3 * db, char *zErrMsg, char *url) {
	char complete_str[2200];
	snprintf(complete_str, 2200,  "UPDATE url SET scanned = 1 WHERE address IS '%s';", url);
	return sqlite3_exec(db, complete_str, callback, 0, &zErrMsg);
		
}

int retrieve_urls(sqlite3 * db, char *zErrMsg, URL * purl) {
	sqlite3_stmt * statement;
	char query[] =  "SELECT id, address, scanned FROM url;";
	int response;
	int i = 0;
	char temp_address[2048];

	response = sqlite3_prepare_v2(db, query, strlen(query), &statement, NULL);
	if(response != SQLITE_OK) {
		sqlite3_close(db);
		printf("RETRIEVE URLS: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(statement);
		return -1;
	}
	while (sqlite3_step(statement) == SQLITE_ROW) {	
		purl[i].id = sqlite3_column_int(statement, 0);
		strcpy(temp_address, (char*)sqlite3_column_text(statement, 1));
		purl[i].address = malloc(sizeof(char) * (strlen(temp_address) + 1));
		strcpy(purl[i].address, temp_address);
		purl[i].scanned = sqlite3_column_int(statement, 2);
		i++;
	}
	sqlite3_finalize(statement);
	return i;
}
/*
int main(void) {
	URL *all_urls = malloc(sizeof(URL));
	sqlite3 *db = 0;
	char *zErrMsg = 0;
	int i;
	
	sqlite3_open_v2("urls.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, "unix");
	create_table(db, zErrMsg);
	retrieve_urls(db, zErrMsg, all_urls);
	for(i=0; i<100000; i++) {
		sql_duplicate(db, zErrMsg, "testsadwqdqecbqwhifbwqdnbqwidbiqwbdncqwejbceihwfghh2io");
		printf("We checked\n");
	}
}
*/
