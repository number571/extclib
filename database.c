#include <stdint.h>
#include <stdlib.h>

#include "database/sqlite3.h"
#include "database.h"

typedef struct database_t {
	sqlite3 *ptr;
} database_t;

extern database_t *database_new(const char *filename) {
	database_t *db;
	int rc;

	db = (database_t*)malloc(sizeof(database_t));
	rc = sqlite3_open(filename, &(db->ptr));

	if (rc != SQLITE_OK) {
		database_free(db);
		return NULL;
	}

	return db;
}

extern void database_free(database_t *db) {
	sqlite3_close(db->ptr);
	free(db);
}

extern int database_set(database_t *db, const char *exec) {
	int rc = sqlite3_exec(db->ptr, exec, NULL, NULL, NULL);
	if (rc != SQLITE_OK) {
		return 1;
	}
	return 0;
}

extern int database_get(
	database_t *db, 
	const char *exec, 
	int(*callback)(void*,int,char**,char**), 
	void *arg
) {
	int rc = sqlite3_exec(db->ptr, exec, callback, arg, NULL);
	if (rc != SQLITE_OK) {
		return 1;
	}
	return 0;
}
