CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -pedantic 

SRC = minishell.c 
OBJ = minishell.o 

all: $(OBJ)
	$(CC) $(CFLAGS) -o minishell $(OBJ)

.PHONY: run

run: all
	./minishell

clean:
	rm -f minishell $(OBJ)
	