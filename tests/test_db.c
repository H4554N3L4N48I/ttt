#include <CUnit/Basic.h>
#include "db.h"

void test_open_database() {
    sqlite3 *db;
    int const result = open_database(&db, "test.db");
    CU_ASSERT(result == SQLITE_OK);
}

void test_create_table() {
    sqlite3 *db;
    open_database(&db, "test.db");
    create_table(db);
}

void cleanup() {
    remove("test.db");
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
