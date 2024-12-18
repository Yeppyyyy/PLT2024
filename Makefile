# Compiler and Flags
CC = gcc
CFLAGS = -std=gnu99 -c -Wall -g
LDFLAGS = -Wall -g
EXECUTABLES = test_anasem test_analex test_anasynt anasem analex anasynt

.PHONY: all clean

# Default target: compile all executables
all: $(EXECUTABLES)

# Build rules for each executable
anasem: anasem.o
	$(CC) $(LDFLAGS) $^ -o $@

test_anasem: test_anasem.o anasem.o anasynt.o analex.o
	$(CC) $(LDFLAGS) $^ -o $@

analex: analex.o
	$(CC) $(LDFLAGS) $^ -o $@

test_analex: test_analex.o analex.o
	$(CC) $(LDFLAGS) $^ -o $@

anasynt: anasynt.o analex.o
	$(CC) $(LDFLAGS) $^ -o $@

test_anasynt: test_anasynt.o anasynt.o analex.o
	$(CC) $(LDFLAGS) $^ -o $@

compilateur: compilateur.o anasynt.o analex.o
	$(CC) $(LDFLAGS) $^ -o $@

test_compilateur: test_compilateur.o compilateur.o anasynt.o analex.o
	$(CC) $(LDFLAGS) $^ -o $@

# General rule for object files
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# Clean up generated files
clean:
	rm -f $(EXECUTABLES) *.o