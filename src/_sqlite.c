#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "_sqlite.h"

/* 
TODO: change hardcoded lengths to the defines
*/


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
		printf("Can't retrieve data: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	while (sqlite3_step(statement) == SQLITE_ROW) {	
		return 1;
	}

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
	sqlite3_stmt * statement;
	int response;
	char query[2100];
	snprintf(query, 2100, "SELECT id FROM url WHERE address IS '%s';", str);
	response = sqlite3_prepare_v2(db, query, strlen(query), &statement, NULL);
	if(response != SQLITE_OK) {
		sqlite3_close(db);
		printf("Can't retrieve data: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	while (sqlite3_step(statement) == SQLITE_ROW) {	
		return 1;	
	}
	return 0;
}


int update_url(sqlite3 * db, char *zErrMsg, char *url) {
	char complete_str[2200];
	snprintf(complete_str, 2200,  "UPDATE url SET scanned = 1 WHERE address IS '%s';", url);
	return sqlite3_exec(db, complete_str, callback, 0, &zErrMsg);
		
}

int main(void) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int response;

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
	response = add_url(db, zErrMsg, "https://en.wikipedia.org/wiki/Main_Page", 1);

	response = update_url(db, zErrMsg, "https://en.wikipedia.org/wiki/Main_Page");

	response = sqlite3_close_v2(db);
	return 0;
}
