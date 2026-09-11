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
./student-database.exe
```

The app creates `students.dat` in the folder from which it is launched. This file contains your local records and is intentionally excluded from Git.

## Data format note

Records are stored as C structures in a binary file. They are intended for use with the same build of this program, not as a portable interchange format.

## Before publishing

Add a license that reflects how you want others to use your work. No license is included because the choice belongs to the project owner.

Once Git is installed and you have created an empty GitHub repository, publish with:

```sh
git init
git add "student database.c" README.md Makefile .gitignore
git commit -m "Initial release"
git branch -M main
git remote add origin https://github.com/YOUR-USERNAME/YOUR-REPOSITORY.git
git push -u origin main
```

The `.gitignore` file keeps the executable, local student records, temporary data, and editor files out of the repository.
