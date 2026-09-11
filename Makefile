CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
TARGET := student-database
SOURCE := student database.c

ifeq ($(OS),Windows_NT)
TARGET := $(TARGET).exe
endif

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o "$@" "$<"

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) student-database student-database.exe *.o *.obj students.tmp students.bak
