#ifndef DB_H
#define DB_H

#include <sqlite3.h>

int open_database(sqlite3 **db, const char *path);

void close_database(sqlite3 *db);

void create_table(sqlite3 *db);

int execute_sql(sqlite3 *db, const char *sql);

#endif
