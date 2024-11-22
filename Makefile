# Compiler and linker
CC=gcc

# Compiler flags
CFLAGS=-Wall -Wextra -pedantic -std=c99

# Target executable
TARGET=kilo

# Default target to compile, run, and clean up
kilo:
	@echo "Compiling."
	@$(CC) $(CFLAGS) -o $(TARGET) kilo.c
	@echo "Executing."
	@./$(TARGET)
	@echo "Cleanup."
	@rm -f $(TARGET)

# A 'clean' target to clean up the compiled files
clean:
	@echo "Cleaning up..."
	@rm -f $(TARGET)
