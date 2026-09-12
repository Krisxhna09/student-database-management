# First-Year Student Database

A beginner-friendly C console application for managing first-year student records with file handling.

## Features

- Add a student with a unique, positive ID
- View all students
- Search, update, and delete by ID
- Store records locally in `students.dat`
- Handle invalid menu entries, blank required fields, long text input, and file-operation failures

## Build

Use a C11-compatible compiler. From the project folder:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 -o student-database.exe "student database.c"
```

On Linux or macOS, use `-o student-database` instead of `-o student-database.exe`.

## Run

```sh
student-database.exe
```

The app creates `students.dat` in the folder from which it is launched. This file contains your local records and is intentionally excluded from Git.

## Data format note

Records are stored as C structures in a binary file. They are intended for use with the same build of this program, not as a portable interchange format.
