#ifndef CMD_HANDLER_H
#define CMD_HANDLER_H

#include <sqlite3.h>

void handle_command(sqlite3 *db, int argc, char *argv[]);

#endif