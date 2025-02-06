#include "db.h"
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
	     "elapsed_time INTEGER NOT NULL DEFAULT 0, "
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

int execute_sql_text_param(sqlite3 *db, const char *sql, const char *param) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, param, -1, SQLITE_STATIC);
    const int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc;
}

int execute_sql_int_param(sqlite3 *db, const char *sql, const int param) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, param);
    const int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return rc;
}

void safe_prepare(sqlite3 *db, sqlite3_stmt **stmt_pt, const char* sql) {
  if (sqlite3_prepare_v2(db, sql, -1, stmt_pt, NULL) != SQLITE_OK) {
    fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    return;
  }
}

