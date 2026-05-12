ifeq ($(OS),Windows_NT)
CC = C:/msys64/ucrt64/bin/gcc.exe
PKG_CONFIG = C:/msys64/ucrt64/bin/pkg-config.exe
else
CC = gcc
PKG_CONFIG = pkg-config
endif

CFLAGS = -Wall -Wextra -std=c11 -g -I./src
LDFLAGS =

TARGET = tictactoe.exe
SRCDIR = src
OBJDIR = obj

SOURCES = $(SRCDIR)/main.c $(SRCDIR)/gtk_ui.c $(SRCDIR)/game.c $(SRCDIR)/minimax.c
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))
GTK_FLAGS := $(shell $(PKG_CONFIG) --silence-errors --cflags --libs gtk+-3.0 2>/dev/null)

all: check-gtk $(TARGET)

check-gtk:
ifeq ($(strip $(GTK_FLAGS)),)
	$(error GTK 3 development files not found. Install gtk+-3.0 and make sure pkg-config can resolve it)
endif

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): check-gtk $(OBJDIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(GTK_FLAGS) $(LDFLAGS)
	@echo "GTK build successful. Run ./$(TARGET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(GTK_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
	@echo "Clean completed"

rebuild: clean all

run: $(TARGET)
ifeq ($(OS),Windows_NT)
	./run_tictactoe.bat
else
	./$(TARGET)
endif

help:
	@echo "Available targets:"
	@echo "  all     - Build the GTK interface"
	@echo "  run     - Build and run the GTK interface"
	@echo "  clean   - Remove build artifacts"
	@echo "  rebuild - Clean and rebuild the GTK interface"
	@echo "  help    - Show this help message"

.PHONY: all check-gtk clean rebuild run help
