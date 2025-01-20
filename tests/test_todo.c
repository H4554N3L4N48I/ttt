#include <CUnit/Basic.h>
#include "todo.h"
#include "db.h"

void test_add_task() {
    sqlite3 *db;
    open_database(&db, "test.db");
    create_table(db);

    add_task(db, "Test TaskTest TaskTest TaskTest TaskTest TaskTest TaskTest TaskTest TaskTest TaskTest Task");

    const char *sql = "SELECT COUNT(*) FROM tasks WHERE description = 'Test Task';";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    CU_ASSERT(sqlite3_step(stmt) == SQLITE_ROW);
    CU_ASSERT(sqlite3_column_int(stmt, 0) == 1);

    sqlite3_finalize(stmt);
    close_database(db);
}

void test_mark_task_done() {
    sqlite3 *db;
    open_database(&db, "test.db");
    create_table(db);

    add_task(db, "Task to be marked done");

    const char *sql = "SELECT task_id FROM tasks WHERE description = 'Task to be marked done';";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_step(stmt);
    int task_id = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    mark_task_done(db, task_id);

    sql = "SELECT status FROM tasks WHERE task_id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, task_id);
    sqlite3_step(stmt);
    CU_ASSERT(sqlite3_column_int(stmt, 0) == 1);

    sqlite3_finalize(stmt);
    close_database(db);
}

void test_delete_task() {
    sqlite3 *db;
    open_database(&db, "test.db");
    create_table(db);

    add_task(db, "Task to be deleted");

    const char *sql = "SELECT task_id FROM tasks WHERE description = 'Task to be deleted';";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_step(stmt);
    int task_id = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    delete_task(db, task_id);

    sql = "SELECT COUNT(*) FROM tasks WHERE task_id = ?;";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, task_id);
    sqlite3_step(stmt);
    CU_ASSERT(sqlite3_column_int(stmt, 0) == 0);

    sqlite3_finalize(stmt);
    close_database(db);
}

void test_list_tasks() {
    sqlite3 *db;
    open_database(&db, "test.db");
    create_table(db);

    add_task(db, "Task 1");
    add_task(db, "Task 2");

    printf("\nExpected output:\n");
    printf("ID  | Description                 | Status   | Created At\n");
    printf("------------------------------------------------------------------\n");
    printf("1   | Task 1                      | Pending  | <timestamp>\n");
    printf("2   | Task 2                      | Pending  | <timestamp>\n");
    printf("\nActual output:\n");

    list_tasks(db);

    close_database(db);
}

void cleanup() {
    remove("test.db");
}


int main() {
    CU_initialize_registry();
    CU_pSuite pSuite = CU_add_suite("Task Management Tests", 0, cleanup);

    CU_add_test(pSuite, "Test Add Task", test_add_task);
    CU_add_test(pSuite, "Test Mark Task Done", test_mark_task_done);
    CU_add_test(pSuite, "Test Delete Task", test_delete_task);
    CU_add_test(pSuite, "Test List Tasks", test_list_tasks);

    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
