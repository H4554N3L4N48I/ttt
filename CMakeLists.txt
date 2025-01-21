cmake_minimum_required(VERSION 3.29)
project(todo_cli_lite C)

set(CMAKE_C_STANDARD 99)

include_directories(src/ /usr/local/include)
link_directories(/usr/local/lib)

add_executable(todo_cli_lite
        src/main.c
        src/db.c
        src/todo.c
        src/cmd_handler.c
)
target_link_libraries(todo_cli_lite sqlite3)

# tests

add_executable(test_db tests/test_db.c src/db.c)
target_link_libraries(test_db cunit sqlite3)


add_executable(test_cmd_handler tests/test_cmd_handler.c src/cmd_handler.c src/db.c src/todo.c)
target_link_libraries(test_cmd_handler cunit sqlite3)