// This code was generated on a MacOS, maybe the libraries
// required are different on other OS.
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

char *trim_whitespaces(char *token); // function to remove leading and trailing whitespaces

int main(void) {

    char begin[] = "Welcome to my mini-shell!\n";
    char prompt[] = "cmd> ";
    char *input_symbol = "<";
    char *output_symbol = ">";
    char *token;

    write(STDOUT_FILENO, begin, sizeof(begin) - 1);

    while(true) {

        write(STDOUT_FILENO, prompt, sizeof(prompt) - 1);
        char buffer[BUFFER_SIZE]; // buffer for the command line
        char *argv[MAX_ARG]; // array of arguments for execve
        char **envp = {NULL}; // environment variables

        if(!fgets(buffer, BUFFER_SIZE, stdin)) {
            // if user pressed Ctrl-D, exit the program
            write(STDOUT_FILENO, "\n", 1);
            break;
        }

        size_t len = strlen(buffer);
        if(len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        if(strlen(buffer) == 0) {
            continue; // if the user pressed enter without typing anything
        }

        // parse the command line for pipes
        char *commands[MAX_PIPES + 1]; // array of commmands between pipes
        int num_commands = 0; // number of commands between pipes
        token = strtok(buffer, "|");

        // store commands between pipes
        while(token != NULL && num_commands < MAX_ARG) {
            commands[num_commands++] = strdup(token);
            token = strtok(NULL, "|");
        }
        commands[num_commands] = NULL;

        // create necessary pipes
        int pipes[MAX_PIPES][2]; // array of pipe descriptors
        for(int i = 0; i < num_commands - 1; i++) {
            if(pipe(pipes[i]) < 0) {
                perror("Pipe failed.\n");
                exit(1);
            }
        }

        // fork and execute each command
        for(int i = 0; i < num_commands; i++) {
            pid_t pid_cmd = fork();
            if(pid_cmd < 0) {
                perror("Fork failed.\n");
                exit(1);
            }
            else if(pid_cmd == 0) {
                // child process
                // if not the first command, read from previous pipe
                if(i > 0) {
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                }
                // if not the last command, write to the next pipe  
                if(i < num_commands - 1) {
                    dup2(pipes[i][1], STDOUT_FILENO);
                }
                // close all pipe fds in child process to avoid leaks
                for(int j = 0; j < num_commands - 1; j++) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }

                // set up input/output redirections for files
                for(int j = 0; commands[i][j] != '\0'; j++) {

                    if(commands[i][j] == *input_symbol) {
                        char *input_path = &commands[i][j + 1];
                        input_path = trim_whitespaces(input_path);
                        int fd = open(input_path, O_RDONLY);
                        if(fd < 0) {
                            perror("Input file not found, try again.\n");
                            exit(1);
                        }
                        dup2(fd, 0);
                        close(fd);
                    }
                    if(commands[i][j] == *output_symbol) {
                        char *output_path = &commands[i][j + 1];
                        output_path = trim_whitespaces(output_path);
                        int fd = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if(fd < 0) {
                            perror("Output file not found, try again.\n");
                            exit(1);
                        }
                        dup2(fd, 1);
                        close(fd);
                    }
                }

                // fill argv array with command path and arguments
                int arg_index = 0;
                char *command_path = strtok(commands[i], " \n");
                argv[arg_index++] = command_path;
                token = command_path;
                while((token = strtok(NULL, " \n")) != NULL && arg_index < MAX_ARG - 1) {
                    if(strcmp(token, input_symbol) == 0 || strcmp(token, output_symbol) == 0) {
                        arg_index++;
                        break;
                    }
                    argv[arg_index++] = token;
                }
                argv[arg_index] = NULL;

                // execute command
                if(access(command_path, F_OK) == 0) {
                execve(command_path, argv, envp);
                close(1);
                close(0);
                }
                else {
                    perror("Command/Path for execution not found.\n");
                    exit(1);
                }
            }
            else {
                // parent process
                // close pipe file descriptors
                if(i > 0) {
                    close(pipes[i - 1][0]);
                    close(pipes[i - 1][1]);
                }
            }
        }

        for(int i = 0; i < num_commands; i++) {
            wait(NULL);
        }

        // free memory
        for(int i = 0; i < num_commands; i++) {
            free(commands[i]);
        }
    };

    return 0; 
}



// Auxiliary functions

char *trim_whitespaces(char *token) {
    // Remove leading whitespace characters
    while (*token && isspace(*token)) {
        token++;
    }

    // Remove trailing whitespace characters
    char* end = token + strlen(token) - 1;
    while (end > token && isspace(*end)) {
        end--;
    }
    *(end + 1) = '\0';

    return token;
}
