install : src/main.c src/db.c src/todo.c src/cmd_handler.c \
	src/db.h src/todo.h src/cmd_handler.h
	cc -o ttt src/*.c -lsqlite3

test_db : tests/test_db.c src/db.c src/db.h
	cc -o test_db tests/test_db.c src/db.c -lsqlite3 -lcunit

test_cmd_handler : tests/test_cmd_handler.c src/cmd_handler.c src/db.c src/todo.c \
	src/cmd_handler.h src/db.h src/todo.h
	cc -o test_cmd_handler tests/test_cmd_handler.c src/cmd_handler.c src/db.c \
		src/todo.c -lsqlite3 -lcunit

clean :
	-rm ttt tasks.db

clean_tests :
	-rm test_*
