cmake_minimum_required(VERSION 3.29)
project(todo_cli_lite C)

set(CMAKE_C_STANDARD 99)


add_executable(todo_cli_lite src/main.c
        src/db.h
        src/todo.h)

# Tests
add_executable(test_db tests/test_db.c src/db.c)
target_link_libraries(test_db cunit)

add_executable(test_todo tests/test_todo.c src/todo.c)
target_link_libraries(test_todo cunit)