#include "../include/utils.h"

char *trim_whitespaces(char *token) {
    // remove leading whitespace characters
    while (*token && isspace(*token)) {
        token++;
    }

    // remove trailing whitespace characters
    char* end = token + strlen(token) - 1;
    while (end > token && isspace(*end)) {
        end--;
    }
    *(end + 1) = '\0';

    return token;
}

void parse_command(char *cmd, t_command *parsed_cmd) {
    memset(parsed_cmd, 0, sizeof(t_command)); // inicialize struct with zeros

    char *token = strtok(cmd, " \n");
    int arg_index = 0;

    while (token != NULL && arg_index < MAX_ARG - 1) {
        if (strcmp(token, "<") == 0) {  // input redirection
            token = strtok(NULL, " \n");
            if (token) parsed_cmd->infile = strdup(token);
        }
        else if (strcmp(token, ">") == 0) {  // output redirection (truncate)
            token = strtok(NULL, " \n");
            if (token) {
                parsed_cmd->outfile = strdup(token);
                parsed_cmd->append = false;
            }
        }
        else if (strcmp(token, ">>") == 0) {  // output redirection (append)
            token = strtok(NULL, " \n");
            if (token) {
                parsed_cmd->outfile = strdup(token);
                parsed_cmd->append = true;
            }
        }
        else {  // command arguments
            parsed_cmd->argv[arg_index++] = token;
        }
        token = strtok(NULL, " \n");
    }
    parsed_cmd->argv[arg_index] = NULL;
}

char *find_executable(char *cmd) {
    if (access(cmd, X_OK) == 0) { // if it's an absolute path
        return strdup(cmd);
    }

    char *path = getenv("PATH");
    if (!path) return NULL;

    char *path_copy = strdup(path); // makes a copy of the PATH variable
    char *dir = strtok(path_copy, ":"); // directories are separated by ':'
    char full_path[MAX_PATH];

    while (dir) {
        // format the full path to the command
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        if (access(full_path, X_OK) == 0) { // if it's an executable file
            free(path_copy);
            return strdup(full_path);
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL; // if the command is not found
}
