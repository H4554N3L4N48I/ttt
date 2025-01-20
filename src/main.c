#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "db.h"
#include "todo.h"

void help() {
    printf("\nUsage :- ");
    printf("\n$ ./todo add \"todo item\"  # Add a new todo ");
    printf("\n$ ./todo ls               # Show remaining todos");
    printf("\n$ ./todo del NUMBER       # Delete a todo");
    printf("\n$ ./todo done NUMBER      # Complete a todo");
    printf("\n$ ./todo help             # Show usage");
    printf("\n$ ./todo report           # Statistics\n");
    exit(1);
}

void handle_command(sqlite3 *db, int argc, char *argv[]) {
    if (argc < 2) {
        help();
    }

    if (strcmp(argv[1], "add") == 0) {
        if (argc < 3) {
            printf("Error: Missing todo item description\n");
            help();
        }
        add_task(db, argv[2]);
    } else if (strcmp(argv[1], "ls") == 0) {
        list_tasks(db);
    } else if (strcmp(argv[1], "del") == 0) {
        if (argc < 3) {
            printf("Error: Missing task ID\n");
            help();
        }
        int del_id = atoi(argv[2]);
        delete_task(db, del_id);
    } else if (strcmp(argv[1], "done") == 0) {
        if (argc < 3) {
            printf("Error: Missing task ID\n");
            help();
        }
        const int done_id = atoi(argv[2]);
        mark_task_done(db, done_id);
    } else if (strcmp(argv[1], "help") == 0) {
        help();
    } else if (strcmp(argv[1], "report") == 0) {
        printf("Report functionality is not implemented yet.\n");
    } else {
        printf("Error: Unknown command '%s'\n", argv[1]);
        help();
    }
}

int main(const int argc, char *argv[]) {
    sqlite3 *db;
    if (open_database(&db, "todo.db") != SQLITE_OK) {
        fprintf(stderr, "Failed to open database.\n");
        return 1;
    }

    create_table(db);

    handle_command(db, argc, argv);

    close_database(db);
    return 0;
}
