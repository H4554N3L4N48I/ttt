#include <sqlite3.h>
#include <stdio.h>
#include <string.h>


void add_task(sqlite3 *db, const char *description) {
    if (!description || strlen(description) == 0) {
        fprintf(stderr, "Error: Task description cannot be empty.\n");
        return;
    }

    const char *sql = "INSERT INTO tasks (description) VALUES (?);";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, description, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_finalize(stmt);
}

void list_tasks(sqlite3 *db) {
    const char *sql = "SELECT task_id, description, status, created_at FROM tasks;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\nID  | Description                 | Status   | Created At\n");
    printf("------------------------------------------------------------------\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int const task_id = sqlite3_column_int(stmt, 0);
        const char *description = (const char *) sqlite3_column_text(stmt, 1);
        int const status = sqlite3_column_int(stmt, 2);
        const char *created_at = (const char *) sqlite3_column_text(stmt, 3);

        printf("%-3d | %-27.27s | %-8s | %s\n",
               task_id,
               description ? description : "(no description)",
               status == 0 ? "Pending" : "Done",
               created_at ? created_at : "(unknown)");
    }
    printf("\n");
    sqlite3_finalize(stmt);
}


void mark_task_done(sqlite3 *db, int task_id) {
    if (task_id <= 0) {
        fprintf(stderr, "Error: Invalid task ID.\n");
        return;
    }

    const char *sql = "UPDATE tasks SET status = 1 WHERE task_id = ?;";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, task_id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return;
    }

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


void delete_task(sqlite3 *db, int task_id) {
    if (task_id <= 0) {
        fprintf(stderr, "Error: Invalid task ID.\n");
        return;
    }

    const char *sql = "DELETE FROM tasks WHERE task_id = ?;";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, task_id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_finalize(stmt);
}
