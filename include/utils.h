#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>

#define MAX_ARG 16 // max number of flags after a command path
#define MAX_PIPES 8 // max number of pipes in a command line
#define BUFFER_SIZE 256 // max number of characters in a command line
#define MAX_PATH 1024 // max number of characters in a path

typedef struct s_command {
    char  *argv[MAX_ARG];
    char  *infile;
    char  *outfile;
    bool  append;
} t_command;

char *find_executable(char *cmd); // function to find the executable path
char *trim_whitespaces(char *token); // function to remove leading and trailing whitespaces
void parse_command(char *cmd, t_command *parsed_cmd); // function to parse command line

#endif // UTILS_H
