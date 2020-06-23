#include <stdio.h>

#include "extclib/database.h"

#define EXEC_SIZE 128

int callback(void *NotUsed, int argc, char **argv, char **azColName);

int main(void) {
    SQLite *db = new_sqlite("database.db");
    set_sqlite(db, 
        "CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name VARCHAR(44) UNIQUE"
        ");"
    );

    char exec[EXEC_SIZE];
    char *names[3] = {"John", "Tom", "Alex"};

    for (size_t i = 0; i < 3; ++i) {
        snprintf(exec, EXEC_SIZE, "INSERT INTO users (name) VALUES ('%s');", names[i]);
        set_sqlite(db, exec);
    }

    get_sqlite(db, "SELECT * FROM users;", callback);

    free_sqlite(db);
    return 0;
}

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i += 2) {
        printf("[ %s = %s, %s = %s ]\n", azColName[i], argv[i], azColName[i+1], argv[i+1]);
    }
    return 0;
}
