#include <stdio.h>

#include "extclib/database.h"

int callback(void *null, int argc, char **argv, char **col);

int main(void) {
    database_t *db = database_new("database.db");
    database_set(db, 
        "CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name VARCHAR(64) UNIQUE"
        ");"
    );

    char exec[BUFSIZ];
    char *names[3] = {"Alice", "Bob", "Eve"};

    for (size_t i = 0; i < 3; ++i) {
        snprintf(exec, BUFSIZ, "INSERT INTO users (name) VALUES ('%s');", names[i]);
        database_set(db, exec);
    }

    database_get(db, "SELECT * FROM users;", callback, NULL);

    database_free(db);
    return 0;
}

int callback(void *null, int argc, char **argv, char **col) {
    for (int i = 0; i < argc; i += 2) {
        printf("[ %s = %s, %s = %s ]\n", col[i], argv[i], col[i+1], argv[i+1]);
    }
    return 0;
}
