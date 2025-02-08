# Overview

Task Time Tracker (`ttt`) is a simple command-line interface (CLI) task manager that allows users to manage their tasks and track time spent on them.

# Usage

```
$ ttt add TASK_NAME    # Add a new task
$ ttt ls               # View a detailed listing of tasks (including their IDs)
$ ttt track TASK_ID    # Start time tracking a task (use CTRL+D to stop)
$ ttt del TASK_ID      # Delete a task
$ ttt done TASK_ID     # Mark a task as done
$ ttt help             # Print help message
$ ttt report           # View a summary of completed and pending tasks
```

# Installation

## Install Dependencies

* Sqlite3
* Make
* Cunit (Optional: to run unit tests)

## Compile the program

* `cd ttt/`
* `make` _produces the executable program_
* `make test_db` and `make test_cmd_handler` _produce unit testing excutables_

# Copyright

`TODO`