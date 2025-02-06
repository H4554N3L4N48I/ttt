#ifndef TODO_H
#define TODO_H

#include <sqlite3.h>

char* format_time(char* str, int seconds);

void add_task(sqlite3 *db, const char *task);

void list_tasks(sqlite3 *db);

void mark_task_done(sqlite3 *db, int task_id);

void print_report(sqlite3 *db);

void delete_task(sqlite3 *db, int task_id);

void track_task(sqlite3 *db, int task_id);

void help();

#endif
