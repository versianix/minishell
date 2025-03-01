CC = gcc

# Compilation flags
CFLAGS = -g -Wall -Wextra -Werror -pedantic -Iinclude

# Source files
SRC = src/minishell.c src/utils.c

# Object files 
OBJ = $(SRC:.c=.o)

# Executable name
TARGET = minishell

# Rule to build the executable
all: $(TARGET)

# Executable dependencies
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compile .o files from .c files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Command to run the executable
.PHONY: run
run: all
	./$(TARGET)

# Clean the project
.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
