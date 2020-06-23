#ifndef EXTCLIB_SQLITE_H_
#define EXTCLIB_SQLITE_H_

#include <stdint.h>

typedef struct SQLite SQLite;

extern SQLite *new_sqlite(char *filename);
extern void free_sqlite(SQLite *db);

extern int8_t set_sqlite(SQLite *db, char *exec);
extern int8_t get_sqlite(SQLite *db, char *exec, int(*callback)(void*,int,char**,char**));

#endif /* EXTCLIB_SQLITE_H_ */
