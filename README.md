# todo-cli-lite

Todo CLI Lite is a simple command-line interface (CLI) task manager that allows users to manage their tasks using SQLite.

It supports adding tasks, listing tasks, marking them as done, and deleting tasks.

## Features

* Add tasks: Add new tasks to your to-do list.
* List tasks: View all tasks with their statuses (Pending or Done).
* Mark tasks as done: Mark tasks as completed.
* Delete tasks: Remove tasks from your to-do list.
* Report: View a summary of completed and pending tasks.

## Usage

```
$ ./todo add "todo item"  # Add a new todo
$ ./todo ls               # Show remaining todos
$ ./todo del NUMBER       # Delete a todo
$ ./todo done NUMBER      # Complete a todo
$ ./todo help             # Show usage
$ ./todo report           # Statistics
```
