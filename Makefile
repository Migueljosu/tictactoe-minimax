# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -I./src
TARGET = tictactoe.exe
SRCDIR = src
OBJDIR = obj

# Source files (all .c files in src directory)
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Default target
all: $(TARGET)

# Create obj directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Link object files to create executable
$(TARGET): $(OBJDIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(CFLAGS)
	@echo "Build successful! Run ./$(TARGET) to start the game"

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJDIR) $(TARGET)
	@echo "Clean completed"

# Rebuild everything
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

# Memory leak check (if valgrind is available)
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  clean    - Remove build files"
	@echo "  rebuild  - Clean and rebuild"
	@echo "  run      - Build and run the program"
	@echo "  valgrind - Check for memory leaks (requires valgrind)"
	@echo "  help     - Show this help message"

.PHONY: all clean rebuild run valgrind help