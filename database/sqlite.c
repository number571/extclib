#include <stdint.h>
#include <stdlib.h>

#include <sqlite3.h>

typedef struct sqlite3 SQLite;

extern SQLite *new_sqlite(char *filename) {
	SQLite *db;
	int rc = sqlite3_open(filename, &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		return NULL;
	}
	return db;
}

extern int8_t set_sqlite(SQLite *db, char *exec) {
	int rc = sqlite3_exec(db, exec, NULL, 0, NULL);
	if (rc != SQLITE_OK) {
		return 1;
	}
	return 0;
}

extern int8_t get_sqlite(SQLite *db, char *exec, int(*callback)(void*,int,char**,char**)) {
	int rc = sqlite3_exec(db, exec, callback, 0, NULL);
	if (rc != SQLITE_OK) {
		return 1;
	}
	return 0;
}

extern void free_sqlite(SQLite *db) {
	sqlite3_close(db);
}

