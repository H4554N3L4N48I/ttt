#include <CUnit/Basic.h>
#include "db.h"

void test_open_database() {
    sqlite3 *db;
    int const result = open_database(&db, "test.db");
    CU_ASSERT(result == SQLITE_OK);
    CU_ASSERT(db != NULL);
    sqlite3_close(db);
}

void test_create_table() {
    sqlite3 *db;
    open_database(&db, "test.db");
    int rc = create_table(db);
    CU_ASSERT(rc == SQLITE_OK);

    const char *query = "SELECT name FROM sqlite_master WHERE type='table' AND name='tasks';";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    CU_ASSERT(rc == SQLITE_OK);

    rc = sqlite3_step(stmt);
    CU_ASSERT(rc == SQLITE_ROW);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

int cleanup() {
    remove("test.db");
    return 0;
}


int main() {
    CU_initialize_registry();
    CU_pSuite pSuite = CU_add_suite("Database Tests", 0, cleanup);

    CU_add_test(pSuite, "Test Open Database", test_open_database);
    CU_add_test(pSuite, "Test Create Table", test_create_table);

    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
