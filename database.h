#ifndef EXTCLIB_DATABASE_H_
#define EXTCLIB_DATABASE_H_

#include <stdint.h>

typedef struct database_t database_t;

extern database_t *database_new(const char *filename);
extern void database_free(database_t *db);

extern int database_set(database_t *db, const char *exec);
extern int database_get(
	database_t *db, 
	const char *exec, 
	int(*callback)(void*,int,char**,char**), 
	void *arg
);

#endif /* EXTCLIB_DATABASE_H_ */
