#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "todo.h"
#include "db.h"


char* format_time(char* str, int seconds) {
  int hours = seconds / 3600;
  seconds = seconds % 3600;

  int minutes = seconds / 60;
  seconds = seconds % 60;

  sprintf(str, "%02dh%02dm%02ds", hours, minutes, seconds);
  return str;
}

void add_task(sqlite3 *db, const char *description) {
    if (!description || strlen(description) == 0) {
        fprintf(stderr, "Error: Task description cannot be empty.\n");
        return;
    }

    const char *sql = "INSERT INTO tasks (description) VALUES (?);";
    const int rc = execute_sql_text_param(db, sql, description);
    if (rc != SQLITE_DONE)
        printf("Error: '%s'", sqlite3_errmsg(db));
}


void mark_task_done(sqlite3 *db, int task_id) {
    if (task_id <= 0) {
        fprintf(stderr, "Error: Invalid task ID.\n");
        return;
    }

    const char *sql = "UPDATE tasks SET status = 1 WHERE task_id = ?;";
    const int rc = execute_sql_int_param(db, sql, task_id);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    }
}


void delete_task(sqlite3 *db, const int task_id) {
    if (task_id <= 0) {
        fprintf(stderr, "Error: Invalid task ID.\n");
        return;
    }

    const char *sql = "DELETE FROM tasks WHERE task_id = ?;";
    const int rc = execute_sql_int_param(db, sql, task_id);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    }
}

void list_tasks(sqlite3 *db) {
    const char *sql = "SELECT task_id, description, status, elapsed_time, created_at FROM tasks;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\nID  | Description                 | Status   | Elapsed Time | Created At\n");
    printf("---------------------------------------------------------------------------------\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int const task_id = sqlite3_column_int(stmt, 0);
        const char *description = (const char *) sqlite3_column_text(stmt, 1);
        int const status = sqlite3_column_int(stmt, 2);
	char elapsed_time_str[10];
	int const elapsed_time = sqlite3_column_int(stmt, 3);
        const char *created_at = (const char *) sqlite3_column_text(stmt, 4);
	
        printf("%-3d | %-27.27s | %-8s | %-13s| %s\n",
               task_id,
               description ? description : "(no description)",
               status == 0 ? "Pending" : "Done",
	       format_time(elapsed_time_str, elapsed_time),
               created_at ? created_at : "(unknown)");
    }
    printf("\n");
    sqlite3_finalize(stmt);
}


void print_report(sqlite3 *db) {
    const char *sql_done = "SELECT COUNT(*) FROM tasks WHERE status = 1;";
    const char *sql_pending = "SELECT COUNT(*) FROM tasks WHERE status = 0;";

    sqlite3_stmt *stmt_done;
    sqlite3_stmt *stmt_pending;

    if (sqlite3_prepare_v2(db, sql_done, -1, &stmt_done, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (sqlite3_prepare_v2(db, sql_pending, -1, &stmt_pending, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt_done);
        return;
    }

    sqlite3_step(stmt_done);
    int done_count = sqlite3_column_int(stmt_done, 0);

    sqlite3_step(stmt_pending);
    int pending_count = sqlite3_column_int(stmt_pending, 0);

    printf("\n  Task Report:\n");
    printf("----------------\n");
    printf("Status  |  Count\n");
    printf("----------------\n");
    printf("Done    |  %d\n", done_count);
    printf("Pending |  %d\n", pending_count);

    sqlite3_finalize(stmt_done);
    sqlite3_finalize(stmt_pending);
}


void help() {
    printf("\nWelcome to the Todo Command Line Interface Task Manager!\n");
    printf("\nUsage:");
    printf("\n$ ./todo add \"todo item\"  # Add a new todo ");
    printf("\n$ ./todo ls               # Show remaining todos");
    printf("\n$ ./todo del NUMBER       # Delete a todo");
    printf("\n$ ./todo done NUMBER      # Complete a todo");
    printf("\n$ ./todo help             # Show usage");
    printf("\n$ ./todo report           # Statistics\n");
    exit(1);
}
