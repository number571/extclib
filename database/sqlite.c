#include <stdint.h>
#include <stdlib.h>

#include <sqlite3.h>

#include "sqlite.h"

extern SQLite_e *new_sqlite(char *filename) {
	sqlite3 *db;
	int rc = sqlite3_open(filename, &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		return NULL;
	}
	return (SQLite_e*)db;
}

extern void free_sqlite(SQLite_e *db) {
	sqlite3_close((sqlite3*)db);
}

extern int8_t set_sqlite(SQLite_e *db, char *exec) {
	int rc = sqlite3_exec((sqlite3*)db, exec, NULL, NULL, NULL);
	if (rc != SQLITE_OK) {
		return 1;
	}
	return 0;
}

extern int8_t get_sqlite(SQLite_e *db, char *exec, int(*callback)(void*,int,char**,char**), void *arg) {
	int rc = sqlite3_exec((sqlite3*)db, exec, callback, arg, NULL);
	if (rc != SQLITE_OK) {
		return 1;
	}
	return 0;
}
