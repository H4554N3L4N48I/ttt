#include "db.h"
#include <sqlite3.h>
#include <stdio.h>

int open_database(sqlite3 **db, const char *path) {
    int const rc = sqlite3_open(path, db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    return SQLITE_OK;
}

void close_database(sqlite3 *db) {
    sqlite3_close(db);
}

void create_table(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS tasks ("
            "task_id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "description TEXT NOT NULL, "
            "status INTEGER NOT NULL DEFAULT 0,"
            "created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP"
            ");";
    int const rc = execute_sql(db, sql);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Can't create table: %s\n", sqlite3_errmsg(db));
}

int execute_sql(sqlite3 *db, const char *sql) {
    char *errmsg = NULL;
    int const rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
    }
    return rc;
}
