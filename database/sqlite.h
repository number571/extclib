#ifndef EXTCLIB_SQLITE_H_
#define EXTCLIB_SQLITE_H_

#include <stdint.h>

typedef struct sqlite3 SQLite_e;

extern SQLite_e *new_sqlite(char *filename);
extern void free_sqlite(SQLite_e *db);

extern int8_t set_sqlite(SQLite_e *db, char *exec);
extern int8_t get_sqlite(SQLite_e *db, char *exec, int(*callback)(void*,int,char**,char**), void *arg);

#endif /* EXTCLIB_SQLITE_H_ */
