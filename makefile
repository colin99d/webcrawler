# Compiler options
CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
EXECUTABLE_NAME=scraper

# Folders
SRC=src
BIN=bin
OBJ=$(BIN)/obj

# Files
SOURCE_FILES= $(SRC)/main.c $(SRC)/requests.c
EXECUTABLE_FILES = $(EXECUTABLE_NAME:%=$(BIN)/%)
OBJECT_FILES     = $(SOURCE_FILES:%.c=$(OBJ)/%.o)
#     http://www.gnu.org/software/make/manual/make.html#Substitution-Refs

build: $(EXECUTABLE_FILES)

clean:
		rm -r -f $(BIN)

# http://www.gnu.org/software/make/manual/make.html#Phony-Targets
.PHONY: build clean

$(EXECUTABLE_FILES): $(OBJECT_FILES)
		@$(CC) $(LDFLAGS) -o $@ $^ -lcurl
		@# ^^^ http://www.gnu.org/software/make/manual/make.html#Automatic-Variables

# http://www.gnu.org/software/make/manual/make.html#Static-Pattern
$(OBJECT_FILES): $(OBJ)/%.o: %.c
		@echo Compiling $<
		@# ^^^ Your terminology is weird: you "compile a .cpp file" to create a .o file.
		@mkdir -p $(@D)
		@# ^^^ http://www.gnu.org/software/make/manual/make.html#index-_0024_0028_0040D_0029
		@$(CC) $(CFLAGS) -o $@ $<
		@# ^^^ Use $(CFLAGS), not $(LDFLAGS), when compiling.
