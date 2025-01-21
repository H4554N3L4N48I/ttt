#include <CUnit/Basic.h>
#include <sqlite3.h>
#include <stdio.h>
#include "db.h"
#include "cmd_handler.h"

sqlite3 *db;

int setup() {
    open_database(&db, "test.db");
    int rc = create_table(db);
    return rc;
}

int cleanup() {
    sqlite3_close(db);
    remove("test.db");
    return 0;
}

int count_tasks() {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT COUNT(*) FROM tasks";
    int count = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        printf("Failed to execute query: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count;
}

void test_add_command() {
    char *argv[] = {"todo_cli_lite", "add", "Test Task"};
    handle_command(db, 3, argv);

    int task_count = count_tasks();
    CU_ASSERT(task_count == 1);
}

void test_done_command() {
    char *argv_add[] = {"todo_cli_lite", "add", "Task to complete"};
    handle_command(db, 3, argv_add);

    sqlite3_stmt *stmt;
    const char *sql = "SELECT task_id FROM tasks ORDER BY task_id DESC LIMIT 1";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_step(stmt);
    int task_id = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);


    char *argv_done[] = {"todo_cli_lite", "done", "2"};
    handle_command(db, 3, argv_done);

    const char *check_sql = "SELECT status FROM tasks WHERE task_id = ?";
    sqlite3_prepare_v2(db, check_sql, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, task_id);
    sqlite3_step(stmt);
    int status = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    CU_ASSERT(status == 1);
    int task_count = count_tasks();
    CU_ASSERT(task_count == 2);
}

void test_del_command() {
    char *argv_add[] = {"todo_cli_lite", "add", "Task to delete"};
    handle_command(db, 3, argv_add);

    int task_count = count_tasks();
    CU_ASSERT(task_count == 3);

    char *argv_del[] = {"todo_cli_lite", "del", "2"};
    handle_command(db, 3, argv_del);

    task_count = count_tasks();
    CU_ASSERT(task_count == 2);
}


int main() {
    CU_initialize_registry();

    CU_pSuite pSuite = CU_add_suite("Command Handler Tests", setup, cleanup);

    CU_add_test(pSuite, "Test Add Command", test_add_command);
    CU_add_test(pSuite, "Test Done Command", test_done_command);
    CU_add_test(pSuite, "Test Delete Command", test_del_command);

    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
