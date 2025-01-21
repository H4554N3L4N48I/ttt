#ifndef DB_H
#define DB_H

#include <sqlite3.h>

#define DATABASE_PATH "tasks.db"
#define TABLE_NAME "tasks"

int open_database(sqlite3 **db, const char *path);

void close_database(sqlite3 *db);

int create_table(sqlite3 *db);

int execute_sql(sqlite3 *db, const char *sql);

int execute_sql_text_param(sqlite3 *db, const char *sql, const char *param);

int execute_sql_int_param(sqlite3 *db, const char *sql, int param);


#endif
