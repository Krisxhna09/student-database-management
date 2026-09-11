// First-year student database using C file handling.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define DATA_FILE "students.dat"
#define TEMP_FILE "students.tmp"
#define BACKUP_FILE "students.bak"
#define FILE_HEADER "FIRST_YEAR_STUDENTS\n"

typedef struct {
    int id;
    char name[50];
    char course[40];
    char email[60];
    char phone[20];
} Student;

int readLine(const char *prompt, char *value, size_t size) {
    size_t length;
    printf("%s", prompt);
    if (fgets(value, (int)size, stdin) == NULL) {
        value[0] = '\0';
        return 0;
    }
    length = strcspn(value, "\n");
    if (value[length] == '\n') {
        value[length] = '\0';
    } else {
        int character;
        while ((character = getchar()) != '\n' && character != EOF) { }
    }
    return 1;
}

int readPositiveId(const char *prompt, int *id) {
    char input[64];
    char *end;
    long value;

    if (!readLine(prompt, input, sizeof(input))) return 0;
    value = strtol(input, &end, 10);
    if (input[0] == '\0' || *end != '\0' || value < 1 || value > INT_MAX) {
        printf("Invalid student ID. Enter a positive whole number.\n");
        return 0;
    }
    *id = (int)value;
    return 1;
}

int readRequiredLine(const char *prompt, char *value, size_t size) {
    if (!readLine(prompt, value, size)) return 0;
    if (value[0] == '\0') {
        printf("This field cannot be blank.\n");
        return 0;
    }
    return 1;
}

int initialiseDataFile(void) {
    FILE *file = fopen(DATA_FILE, "rb");
    char header[sizeof(FILE_HEADER)];
    if (file != NULL) {
        if (fgets(header, sizeof(header), file) == NULL || strcmp(header, FILE_HEADER) != 0) {
            fclose(file);
            fprintf(stderr, "The data file is invalid or was created by an incompatible version.\n");
            return 0;
        }
        fclose(file);
        return 1;
    }
    file = fopen(DATA_FILE, "wb");
    if (file != NULL) {
        int writeResult = fputs(FILE_HEADER, file);
        int closeResult = fclose(file);
        int success = writeResult != EOF && closeResult == 0;
        if (!success) fprintf(stderr, "Could not create the data file.\n");
        return success;
    }
    fprintf(stderr, "Could not create the data file.\n");
    return 0;
}

void skipFileHeader(FILE *file) {
    char header[40];
    fgets(header, sizeof(header), file);
}

int studentExists(int id) {
    FILE *file = fopen(DATA_FILE, "rb");
    Student student;
    if (file == NULL) return 0;
    skipFileHeader(file);
    while (fread(&student, sizeof(Student), 1, file) == 1) {
        if (student.id == id) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void printStudent(const Student *student) {
    printf("\nID: %d\nName: %s\nCourse: %s\nEmail: %s\nPhone: %s\n",
           student->id, student->name, student->course,
           student->email, student->phone);
}

void addStudent(void) {
    FILE *file;
    Student student;
    int writeResult, closeResult;

    printf("\n--- Add Student ---\n");
    if (!readPositiveId("Student ID: ", &student.id)) return;
    if (studentExists(student.id)) {
        printf("That student ID already exists.\n");
        return;
    }
    if (!readRequiredLine("Full name: ", student.name, sizeof(student.name)) ||
        !readRequiredLine("Course: ", student.course, sizeof(student.course)) ||
        !readRequiredLine("Email: ", student.email, sizeof(student.email)) ||
        !readRequiredLine("Phone: ", student.phone, sizeof(student.phone))) return;

    file = fopen(DATA_FILE, "ab");
    if (file == NULL) {
        printf("Could not open the data file.\n");
        return;
    }
    writeResult = fwrite(&student, sizeof(Student), 1, file);
    closeResult = fclose(file);
    if (writeResult != 1 || closeResult != 0) {
        printf("Could not save the student record.\n");
        return;
    }
    printf("Student added successfully.\n");
}

void viewStudents(void) {
    FILE *file = fopen(DATA_FILE, "rb");
    Student student;
    int count = 0;
    printf("\n--- Student List ---\n");
    if (file == NULL) {
        printf("No student records found.\n");
        return;
    }
    skipFileHeader(file);
    while (fread(&student, sizeof(Student), 1, file) == 1) {
        printStudent(&student);
        count++;
    }
    fclose(file);
    if (count == 0) printf("No student records found.\n");
}

void searchStudent(void) {
    FILE *file = fopen(DATA_FILE, "rb");
    Student student;
    int id, found = 0;
    if (!readPositiveId("\nStudent ID to search: ", &id)) return;
    if (file == NULL) {
        printf("No student records found.\n");
        return;
    }
    skipFileHeader(file);
    while (fread(&student, sizeof(Student), 1, file) == 1) {
        if (student.id == id) {
            printf("\n--- Student Found ---\n");
            printStudent(&student);
            found = 1;
            break;
        }
    }
    fclose(file);
    if (!found) printf("Student not found.\n");
}

void updateStudent(void) {
    FILE *file = fopen(DATA_FILE, "rb+");
    Student student;
    int id, found = 0;
    if (!readPositiveId("\nStudent ID to update: ", &id)) return;
    if (file == NULL) {
        printf("No student records found.\n");
        return;
    }
    skipFileHeader(file);
    while (fread(&student, sizeof(Student), 1, file) == 1) {
        if (student.id == id) {
            found = 1;
            printf("Leave a field blank to retain its current value.\n");
            char value[60];
            if (!readLine("New full name: ", value, sizeof(value))) {
                fclose(file);
                printf("Input ended. Update cancelled.\n");
                return;
            }
            if (strlen(value)) snprintf(student.name, sizeof(student.name), "%s", value);
            if (!readLine("New course: ", value, sizeof(value))) {
                fclose(file);
                printf("Input ended. Update cancelled.\n");
                return;
            }
            if (strlen(value)) snprintf(student.course, sizeof(student.course), "%s", value);
            if (!readLine("New email: ", value, sizeof(value))) {
                fclose(file);
                printf("Input ended. Update cancelled.\n");
                return;
            }
            if (strlen(value)) snprintf(student.email, sizeof(student.email), "%s", value);
            if (!readLine("New phone: ", value, sizeof(value))) {
                fclose(file);
                printf("Input ended. Update cancelled.\n");
                return;
            }
            if (strlen(value)) snprintf(student.phone, sizeof(student.phone), "%s", value);
            if (fseek(file, -(long)sizeof(Student), SEEK_CUR) != 0 ||
                fwrite(&student, sizeof(Student), 1, file) != 1) {
                fclose(file);
                printf("Could not update the student record.\n");
                return;
            }
            printf("Student updated successfully.\n");
            break;
        }
    }
    fclose(file);
    if (!found) printf("Student not found.\n");
}

void deleteStudent(void) {
    FILE *file = fopen(DATA_FILE, "rb");
    FILE *temporary;
    Student student;
    int id, found = 0, writeFailed = 0, readFailed;
    if (!readPositiveId("\nStudent ID to delete: ", &id)) return;
    if (file == NULL) {
        printf("No student records found.\n");
        return;
    }
    temporary = fopen(TEMP_FILE, "wb");
    if (temporary == NULL) {
        fclose(file);
        printf("Could not create a temporary file.\n");
        return;
    }
    skipFileHeader(file);
    if (fputs(FILE_HEADER, temporary) == EOF) writeFailed = 1;
    while (fread(&student, sizeof(Student), 1, file) == 1) {
        if (student.id == id) found = 1;
        else if (fwrite(&student, sizeof(Student), 1, temporary) != 1) writeFailed = 1;
    }
    readFailed = ferror(file);
    if (fclose(file) != 0) readFailed = 1;
    if (fclose(temporary) != 0) writeFailed = 1;
    if (readFailed) writeFailed = 1;
    if (writeFailed) {
        remove(TEMP_FILE);
        printf("Could not prepare the updated data file.\n");
        return;
    }
    if (found) {
        if (rename(DATA_FILE, BACKUP_FILE) != 0) {
            remove(TEMP_FILE);
            printf("Could not back up the data file; deletion was cancelled.\n");
            return;
        }
        if (rename(TEMP_FILE, DATA_FILE) != 0) {
            rename(BACKUP_FILE, DATA_FILE);
            printf("Could not replace the data file; the original was restored.\n");
            return;
        }
        if (remove(BACKUP_FILE) != 0) {
            printf("Student deleted, but the backup file could not be removed.\n");
            return;
        }
        printf("Student deleted successfully.\n");
    } else {
        remove(TEMP_FILE);
        printf("Student not found.\n");
    }
}

int main(void) {
    char input[64];
    char *end;
    long selected;
    int choice = 0;
    if (!initialiseDataFile()) return EXIT_FAILURE;
    do {
        printf("\nFIRST-YEAR STUDENT DATABASE\n");
        printf("1. Add Student\n2. View Students\n3. Search\n4. Update\n5. Delete\n6. Exit\n");
        if (!readLine("Choose an option (1-6): ", input, sizeof(input))) {
            printf("\nInput ended. Goodbye!\n");
            break;
        }
        selected = strtol(input, &end, 10);
        if (input[0] == '\0' || *end != '\0' || selected < 1 || selected > 6) {
            printf("Invalid option.\n");
            continue;
        }
        choice = (int)selected;
        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Goodbye!\n"); break;
            default: printf("Invalid option. Please choose 1 to 6.\n");
        }
    } while (choice != 6);
    return 0;
}
