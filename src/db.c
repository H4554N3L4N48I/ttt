#include "db.h"
#include <sqlite3.h>
#include <stdio.h>

int open_database(sqlite3 **db, const char *path) {
    int rc = sqlite3_open(path, db);
    return rc;
}

void close_database(sqlite3 *db) {
    sqlite3_close(db);
}

int create_table(sqlite3 *db) {
    char sql[256];
    snprintf(sql, sizeof(sql),
             "CREATE TABLE IF NOT EXISTS %s ("
             "task_id INTEGER PRIMARY KEY AUTOINCREMENT, "
             "description TEXT NOT NULL, "
             "status INTEGER NOT NULL DEFAULT 0, "
             "created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP"
             ");",
             TABLE_NAME);

    return execute_sql(db, sql);
}

int execute_sql(sqlite3 *db, const char *sql) {
    char *errmsg = NULL;
    int rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
    }
    return rc;
}
