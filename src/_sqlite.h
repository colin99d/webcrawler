#include <sqlite3.h>

#define MAX_URL_LENGTH 2048

typedef struct url {
	int id;
	char *address;
	int scanned;
} URL;

int sql_table_duplicate(sqlite3 *db, char *zErrMsg, char *table);
int create_table(sqlite3 *db, char *zErrMsg);
int drop_table(sqlite3 *db, char *zErrMsg);
int add_url(sqlite3 *db, char *zErrMsg, char *url, int first);
int sql_duplicate(sqlite3 *db, char *zErrMsg, char* str);
int update_url(sqlite3 * db, char *zErrMsg, char *url);
int retrieve_urls(sqlite3 * db, char *zErrMsg, URL * purl);
